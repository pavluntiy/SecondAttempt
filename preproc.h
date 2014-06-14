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
	
	int currentPosition;
	char currentChar;
	string input;
	string output;
	vector<Directive> directives;
	Position sourcePosition;

	bool ignore;

	map<string, string> defined;

//	Log 

	Preprocessor (string input){
		this->ignore =  false;
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

	set<string> includedFiles;

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
					try {
						pr.preprocess();
					}
					catch(PreprocessorException pe){
						throw PreprocessorException("In file " + cf + ", included on " + sourcePosition.toString() + " happened:\n\t" + pe.what());
					}

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

			if(args.size() > 0 && args[0] == "@defined"){
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
			else{
				for(auto it : args){
					if(it[0] == '$' && it.size() > 1){

						string tmp = it.substr(1, it.size() - 1);
						if(!defined.count(tmp)){
							throw PreprocessorException("No such ID " + it + ", position "+ sourcePosition.toString());
						}
						if(defined[tmp] == ""){
							buffer += "EMPTY_SEQUENCE";
						}
						buffer += defined[tmp];
					}
					else {
						buffer += it;
					}
					buffer += ' ';
				}
			}
			cout << buffer << '\n';

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
						buffer += args[i];
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
						buffer += args[i];
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



		if(name == "ifdef" && !ignore || name == "elif"){
			if(args.size() != 1){
				throw PreprocessorException("Incorrect number of arguments \'" + name + "\', position " + sourcePosition.toString());
			}

			ignore = !(defined.count(args[0]));
		}

		if(name == "else" || name == "endif"){
			ignore = false;
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

			consume();	
		}
	}

	void getWS(){
		while(Alphabet::isIdle(currentChar)){
			consume();
		}
	}
};
#endif