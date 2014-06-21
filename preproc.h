#ifndef PREPROC_H
#define PREPROC_H
#include "preprocexception.h"
//#include "log.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "alphabet.h"
#include "position.h"
#include <set>
#include <map>

#include <string>
#include <vector>

class Preprocessor {
public:
	class Directive {
	public:
		string text;
		Position position;

		Directive(string text = "", Position position = {0, 0}){
			this->text = text;
			this->position = position;
		}

		~Directive(){

		}
	};

	struct Macro {
		int argNum;
		string name;

		string body;

		Macro (string name = "", int argNum = 0, string body = ""){
			this->name = name;
			this->argNum = argNum;
			this->body = body;
		}

	};
	
	int currentPosition;
	char currentChar;
	string input;
	string output;
	vector<Directive> directives;
	Position sourcePosition;

	bool ignore, ignoreElse;

	map<string, string> defined;

	map<string, Macro> macroes;

//	Log 

	Preprocessor (string input){
		this->ignore =  false;
		this->ignoreElse = false;
		this->input = input;
		this->currentPosition = 0;
		this->currentChar = input[currentPosition];
		this->output = "";
	}

	~Preprocessor(){

	}
	
	
	Directive getDirective(){
		std::string buffer = "";
		while (!Alphabet::isNewline(currentChar) && !Alphabet::isEof(currentChar) && !find("//") && !find("/*")){
			buffer += currentChar;
			consume();
		}
		return Directive(buffer, sourcePosition);
	}

	void consume (){
		if (Alphabet::isNewline(currentChar)){
			sourcePosition.line ++;
			sourcePosition.linePosition = 1;
		}
		else {
			sourcePosition.linePosition++;
		}

		if(!ignore && currentChar != -1){
			output += currentChar;
		}
		currentPosition++; 
		
		
		
		if ( currentPosition >= (int)input.length() )
			currentChar = EOF;
		else {
			currentChar = input[currentPosition]; 
		}
	}

	void consumeWithoutCopy (){
		if (Alphabet::isNewline(currentChar)){
			sourcePosition.line ++;
			sourcePosition.linePosition = 1;
		}
		else {
			sourcePosition.linePosition++;
		}

		currentPosition++; 
		
		
		
		if ( currentPosition >= (int)input.length() )
			currentChar = EOF;
		else {
			currentChar = input[currentPosition]; 
		}
	}

	set<string> includedFiles;

	


	Macro readMacro (vector<string> args){


		if(args.size() != 2 && args[1].size() <= 1 && args[1][0] != '#'){
			throw PreprocessorException("Wrong definition of macro on " + sourcePosition.toString());
		}

		Macro macro(args[0]);
		

		
		stringstream ss(args[1].substr(1, args[1].size()));
		ss >> macro.argNum;
		if(ss.fail()){
			throw PreprocessorException("Wrong number of arguments on " + sourcePosition.toString());
		}

		while (!find("#endmacro")){
			macro.body += currentChar;
			consumeWithoutCopy ();
		}



		return macro;

	}

	void apply(Directive directive){

		stringstream ss(directive.text);

		string name, cur;

		ss >> name;

		vector<string> args;
		while(ss >> cur){
				args.push_back(cur);
			}
	//	cout << cur;
		if(name == "include" && !ignore){
	//		cout << "ololo!";
			for(auto cf: args){
				if(!includedFiles.count(cf)){
					includedFiles.insert(cf);
					ifstream in(cf);

					if(in.fail()){
						throw PreprocessorException("Failed to open file " + cf + " on position " + sourcePosition.toString());
					}
					string includeInput = "";
					while (!in.eof()) { 
						includeInput += in.get(); 
					}

					Preprocessor pr(includeInput);
					pr.includedFiles = this->includedFiles;
					pr.defined = this->defined;
					pr.macroes = this->macroes;
					try {
						pr.preprocess();
					}
					catch(PreprocessorException pe){
						throw PreprocessorException("In file " + cf + ", included on " + sourcePosition.toString() + " happened:\n\t" + pe.what());
					}

					this->macroes = pr.macroes;
					this->defined = pr.defined;
					this->includedFiles = pr.includedFiles;
					output += '\n';
					output += pr.output;
					in.close();
				}
			}
			
		}

		if(name == "halt" && !ignore){
			string buffer = "";

			for(auto it : args){
				buffer += it;
				buffer += ' ';
			}

			if(buffer != "")
				throw PreprocessorException("Halted on position " + sourcePosition.toString() + " with message: " + buffer);
			else 
				throw PreprocessorException("Halted on position " + sourcePosition.toString());
		}

		if(name == "print" && !ignore){
			string buffer = "";

			if(args.size() > 0 && args[0] == "#defined"){
				buffer = "Defined IDs: \n";
				for(auto it: defined){
					if(it.second == ""){
						buffer +=  "\tID: " + it.first +  ", substitution: EMPTY_SEQUENCE\n";
					}
					else{
						buffer +=  "\tID: " + it.first +  ", substitution: " + it.second + "\n";
					}
				}

				if(defined.size() == 0){
					buffer += "none";
				}
			}

			if(args.size() > 0 && args[0] == "#macroes"){
				buffer = "Defined macroes: \n";
				for(auto it: macroes){
					buffer +=  "Name: " + it.second.name +  "\n Number of arguments: " + to_string(it.second.argNum) + "\nBody:\n" + it.second.body + "\nendmacro\n";
				}

				if(macroes.size() == 0){
					buffer += "none";
				}
			}
			else{
				for(auto it : args){
					if(it[0] == '$' && it.size() > 1){

						string tmp = it.substr(1, it.size());
						if(!defined.count(tmp)){
							throw PreprocessorException("No such ID " + it + ", position "+ sourcePosition.toString());
						}
						if(defined[tmp] == ""){
							buffer += "EMPTY_SEQUENCE";
						}
						buffer += defined[tmp];
					}
					else {
							if(it == "#ws"){
								buffer += ' ';
							}
							else
							if(it == "#nline"){
								buffer += '\n';
							}
							else 
							if(it == "#tab"){
								buffer += '\t';
							}
							else{		
								buffer += it;
							}
					}
				}
			}
			cout << buffer;

		}

		if(name == "define"  && !ignore ){
			if(args.size() == 0){
				throw PreprocessorException("Empty \'define\' on " + sourcePosition.toString());
			}
			else  if(!defined.count(args[0])){

				if(args.size() == 1){
					defined.insert(make_pair(args[0], ""));
				}
				else {
					string buffer = "";
					for(int i = 1; i < args.size(); ++i){
						if(args[i][0] == '$' && args[i].size() > 1){
							buffer += defined[args[i].substr(1, args[i].size() - 1)];
						}
						else {
							if(args[i] == "#ws"){
								buffer += ' ';
							}
							else
							if(args[i] == "#nline"){
								buffer += '\n';
							}
							else 
							if(args[i] == "#tab"){
								buffer += '\t';
							}
							else{		
								buffer += args[i];
							}
						}
					}
					defined.insert(make_pair(args[0], buffer));
				}
			}
			else {
				throw PreprocessorException("Redefine of defined ID on " + sourcePosition.toString() + " (use \'redef\' instead)");
			}
		}

		if(name == "redef" && !ignore){

			if(args.size() == 0){
				throw PreprocessorException("Empty \'redef\' on " + sourcePosition.toString());
			}
			else if(defined.count(args[0])){

				if(args.size() == 1){
					defined[args[0]] =  "";
				}
				else {
					string buffer = "";
					for(int i = 1; i < args.size(); ++i){
						if(args[i][0] == '$' && args[i].size() > 1){
							buffer += defined[args[i].substr(1, args[i].size() - 1)];
						}
						else {
							if(args[i] == "#ws"){
								buffer += ' ';
							}
							else
							if(args[i] == "#nline"){
								buffer += '\n';
							}
							else 
							if(args[i] == "#tab"){
								buffer += '\t';
							}
							else{		
								buffer += args[i];
							}
						}
					}
					defined[args[0]] = buffer;
				}
			}
			else {
				throw PreprocessorException("Redefine of undefined ID on " + sourcePosition.toString());
			}
		}

		if(name == "undef" && !ignore){
			if(args.size() != 1){
				throw PreprocessorException("Wrong number of arguments for \'undef\' on " + sourcePosition.toString());
			}
			else if(defined.count(args[0])){

				
						defined.erase(args[0]);
	
				}
				else {
					throw PreprocessorException("Undefining non-existing ID \'" + args[0] + "\' on " + sourcePosition.toString());
				}
					
		}

		if(name == "read" && !ignore){
			for(auto it: args){
				string a;
				cin >> a;
				defined[it] = a;
			}
		}

		if(name == "cut" && !ignore){
			if(args.size() != 1){
				throw PreprocessorException("Wrong number of arguments in \'cut\' on " + sourcePosition.toString());
			}
			if(args[0][0] != '$' || args[0].size() <= 1){
				throw PreprocessorException("Unrecognized Id in \'cut\' on " + sourcePosition.toString());
			}

			auto key = args[0].substr(1, args[0].size());

			if(!defined.count(key)){
				throw PreprocessorException("Unrecognized Id in \'cut\' on " + sourcePosition.toString());
			}

			auto value = defined[key];

			if(value != ""){
				defined[key] = value.substr(0, value.size() - 1);
			}

			
		}


		if(name == "ifdef" && !ignore || name == "elif"){
			if(args.size() != 1){
				throw PreprocessorException("Incorrect number of arguments \'" + name + "\', position " + sourcePosition.toString());
			}

			ignore = !(defined.count(args[0]));
			ignoreElse = !ignore;
		}

		if(name == "ifndef" && !ignore || name == "elnif" && !ignoreElse){
			if(args.size() != 1){
				throw PreprocessorException("Incorrect number of arguments \'" + name + "\', position " + sourcePosition.toString());
			}

			ignore = (defined.count(args[0]));
			ignoreElse = !ignore;
		}


		if(name == "ifnzero" && !ignore || name == "elnzero" && !ignoreElse ){

			if(args.size() != 1){
				throw PreprocessorException("Incorrect number of arguments \'" + name + "\', position " + sourcePosition.toString());
			}

			if(args[0] == "0" || args[0] == "false" || args[0] == ""){
				ignore = true;
				ignoreElse = !ignore;
			}
			else if(args[0][0] == '$' && args[0].size() > 1){
				string tmp = args[0].substr(1, args[0].size());
				if(!defined.count(tmp)){
					throw PreprocessorException("No such ID " + tmp + ", position "+ sourcePosition.toString());
				}

				tmp = defined[tmp];

				if(tmp == "0" || tmp == "false" || tmp == ""){
					ignore = true;
					ignoreElse = !ignore;
				}
				else {
					ignore = false;
					ignoreElse = !ignore;
				}


			}
			else {
				ignore = false;
				ignoreElse = !ignore;
			}
		}

		if(name == "else"){
			ignore = ignoreElse;
			ignoreElse = false;
		}

		if(name == "endif"){
			ignore = false;
			ignoreElse = false;
		}

		if(name == "macro" && !ignore){
			if(args.size() != 2){
				throw PreprocessorException("Wrong definition of macro on " + sourcePosition.toString());
			}

			if(macroes.count(args[0])){
				throw PreprocessorException("Redefininition of macro \'" + args[0] + "\' on " + sourcePosition.toString());
			}
			macroes[args[0]] = readMacro(args);
		}

	}

	bool get(std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != input[currentPosition + i])
				return false;
		}

		for (int i = 0; i < (int) text.size(); i++)
			consume();

		return true;
	}

	bool getWithoutCopy(std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != input[currentPosition + i])
				return false;
		}

		for (int i = 0; i < (int) text.size(); i++)
			consumeWithoutCopy();

		return true;
	}

	void getComment (){
		while (!Alphabet::isNewline(currentChar) && !Alphabet::isEof(currentChar)){
			consume();
		}
	}

	void getMultyLineComment () {
		while (!get("*/")){
			if (Alphabet::isEof(currentChar)){
				throw PreprocessorException("Unexpected EOF" + sourcePosition.toString());
			}
			consume();
		}
		if (!Alphabet::isEof(currentChar)){
			consume();
		}
	}

	bool find(std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != input[currentPosition + i])
				return false;
		}
		return true;
	}

	void skip(int n){
		for(int i = 1; i <= n; ++i){
			if(currentChar == EOF){
				return;
			}
			if (Alphabet::isNewline(currentChar)){
				sourcePosition.line ++;
				sourcePosition.linePosition = 1;
			}
			else {
				sourcePosition.linePosition++;
			}

			currentPosition++; 
				
			if ( currentPosition >= (int)input.length() )
				currentChar = EOF;
			else {
				currentChar = input[currentPosition]; 
			}
		}
	}

	char prev(){
		if(currentPosition > 0){
			return input[currentPosition - 1];
		}

		return '\0';
	}

	char next(int n = 1){
		if(currentPosition + n < (int)input.length()){
			return input[currentPosition + n];
		}

		return (char) -1;
	}


	bool substitute(string what){

		if( find(what) && (!Alphabet::isIdCharacter(next(what.size())) && !Alphabet::isIdCharacter(prev()) ||
		 	what == "" || 
		 	!Alphabet::isIdCharacter(what[0]) )) 
		{
			this->output += defined[what];
			skip(what.size());
			return true;
		}
		return false;

	}

	vector<string> getMacroArgs(const Macro& macro){

		
		vector<string> result;

	
		if(macro.argNum == 0){
			return vector<string>();
		}


		while(Alphabet::isIdle(currentChar)){

			consumeWithoutCopy();
		}

	//	cout << "success\n";
	//	return result;



		if(currentChar != '('){
			throw PreprocessorException("Error substituting macro \'" + macro.name + "\' to " + sourcePosition.toString() + 
				" got " + currentChar + " expected (");
		}

		

		//	consumeWithoutCopy();

		string buffer = "";




		
		while(currentChar != ')' && currentChar != EOF){

			consumeWithoutCopy();
			while(Alphabet::isIdle(currentChar)){

				consumeWithoutCopy();
			}

			while(currentChar != ',' && currentChar != ')' && !Alphabet::isIdle(currentChar)){
				buffer += currentChar;
				
				consumeWithoutCopy();
			}

			//cout << "success\n";
			//cout << currentChar;
		//	cout << buffer;

			if(buffer != "")
				result.push_back(buffer);
			buffer = "";
		}

		consumeWithoutCopy();

	// /	cout << "success\n";

	//	cout << "AD" << macro.argNum;
		if(result.size() != macro.argNum){
			throw PreprocessorException("Wrong number of arguments in macro substitution \'" + macro.name + "\' on " + sourcePosition.toString());
		}

		return result;

	}

	string setArgs(const Macro &macro){


		auto arglist = getMacroArgs(macro);
		//return "asdf";
		string result = "", buffer = "";

		for(int i = 0; i < macro.body.size(); ++i){

			if(macro.body[i] == '#' && i + 1 < macro.body.size()  && Alphabet::isDigit(macro.body[i + 1])){
				++i;
				while(Alphabet::isDigit(macro.body[i])){
					buffer += macro.body[i];
					++i;
				}

				int curArg = stoi(buffer);

				//cout << curArg << '\n';

				if(curArg >= macro.argNum){
					throw PreprocessorException("Wrong number of argument in definition of macro \'" + macro.name + "\'");
				}

				result += arglist[curArg];
				buffer = "";
			}
		//	else {
				result += macro.body[i];
				
		//	}
		}

		//cout << result;
		return result;

	}

	bool applyMacro(const Macro &macro){
		if( find(macro.name) && !Alphabet::isIdCharacter(next(macro.name.size())) && !Alphabet::isIdCharacter(prev())) 
		{	
			getWithoutCopy(macro.name);

			this->output += setArgs(macro);
	//		skip(macro.name.size());
	//		return true;
		}
		return false;

	}


	void preprocess(){
		Directive currentDirective;
		while (currentChar != EOF){
			
			if(currentChar == '#'){
				consume();
				currentDirective = getDirective();
				apply(currentDirective);
				directives.push_back(currentDirective);
			}
			else
			if (get("/*")){
				getMultyLineComment();
			}
			else
			if (get("//")){
				getComment();
			}

			bool sust = false;

			for(auto it: defined){
				if(substitute(it.first)){
					sust = true;
				}
			}

			for(auto it: macroes){
				//cout << "asdfsadf";
				if(applyMacro(it.second)){
					sust = true;
				}
			}

			if(!sust){
				consume();
			}	

			
		}
	}

	void getWS(){
		while(Alphabet::isIdle(currentChar)){
			consume();
		}
	}
};
#endif