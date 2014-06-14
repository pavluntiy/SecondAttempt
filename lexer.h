#include <iostream>
#include <fstream>
#include <stdio.h>
#include "token.h"
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include "alphabet.h"
#include "parserexception.h"

using namespace std;

class Lexer {

	string input;
	char currentChar;


	set<string> keywords;
	vector<string> additionalOperators;
public:
	vector<Token> output;

	int currentPosition;
	Token::Position sourcePosition;


	Lexer(string input){
		this->sourcePosition = Token::Position(1, 1);
		this->currentPosition = 0;
		this->input = input;
		this->currentChar = input[this->currentPosition];

		ifstream in ("keywords");
		string keyword;
		while(in >> keyword){
			keywords.insert(keyword);
		}
		in.close();

	}

	Token tryAndGetHexadecimal(bool zeroFound){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		bool isFloat = false;
		if (currentChar == 'X' || currentChar == 'x'){
			if (zeroFound){
			buffer = "0x";
				consume();
				if (!Alphabet::isHexadecimalDigit(currentChar)){
					throw ParserException("Hexadecimal not recognized " + startPosition.toString() + "\n");
				}
				while (Alphabet::isHexadecimalDigit(currentChar) || currentChar == '_' || currentChar == '.' && !find("..")){
					if (currentChar == '.'){
						isFloat = true;
					}
					buffer += currentChar;
					consume();
				}
			}
			buffer += std::string(" (hexadecimal)");
			if (isFloat){
				return Token(Token::FLOAT, buffer);
			}
			return Token(Token::INT, buffer);
		}

		return Token(Token::NONE);
	}

	Token tryAndGetBinar(bool zeroFound){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		bool isFloat = false;
		if (currentChar == 'B' || currentChar == 'b'){
			if (zeroFound){
				buffer = "0b";
				consume();
				if (!Alphabet::isBinaryDigit(currentChar)){
					throw ParserException("Binar not recognized " + startPosition.toString()  + "\n");
				}
				while (Alphabet::isBinaryDigit(currentChar) || currentChar == '_' || currentChar == '.' && !find("..")){
					if (currentChar == '.'){
						isFloat = true;
					}
					buffer += currentChar;
					consume();
				}

			}

			if(Alphabet::isDigit(currentChar)){
				throw ParserException("Nonbinar digits in binar number " + sourcePosition.toString());
			}

			buffer += std::string(" (binar)");
			if (isFloat){
				return Token(Token::FLOAT, buffer, startPosition);
			}
			return Token(Token::INT, buffer, startPosition);
		}

		return Token(Token::NONE);
	}

	Token tryAndGetOctal(bool zeroFound){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		bool isFloat = false;

		if (zeroFound){
			buffer = "0";
			while (Alphabet::isOctalDigit(currentChar)|| currentChar == '_' || currentChar == '.' && !find("..")){
					if (currentChar == '.'){
						isFloat = true;
					}
					buffer += currentChar;
					consume();							
			}

			if(buffer == "0"){
				return Token(Token::INT, "0", sourcePosition);
			}
			buffer += std::string(" (octal)");



			if(Alphabet::isDigit(currentChar)){
				throw ParserException("Nonoctal digits in octal number " + sourcePosition.toString());
			}
			if (isFloat){
				return Token(Token::FLOAT, buffer, startPosition);
			}
			return Token(Token::INT, buffer, startPosition);
		}

		return Token(Token::NONE);
	}


	Token tryAndGetDecimal(bool zeroFound){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		bool isFloat = false;
		if (Alphabet::isDigit(currentChar)){
			while (Alphabet::isDigit(currentChar)|| currentChar == '_' || currentChar == '.' && !find("..")){
				if (currentChar == '.'){
							isFloat = true;
						}
						buffer += currentChar;
						consume();							
			}


			if (isFloat){
				return Token(Token::FLOAT, buffer, startPosition);
			}
			return Token(Token::INT, buffer, startPosition);

		}

		return Token(Token::NONE);
	}


	Token tryAndGetNumeric(){
		Token numeric;
		bool zeroFound = false;

		if (currentChar == '0'){
			zeroFound = true;
			consume();
		}

		numeric = tryAndGetHexadecimal(zeroFound);
		if (numeric != Token(Token::NONE)){
			return numeric;
		}

		numeric = tryAndGetBinar(zeroFound);
		if (numeric != Token(Token::NONE)){
			return numeric;
		}

		numeric = tryAndGetOctal(zeroFound);
		if (numeric != Token(Token::NONE)){
			return numeric;
		}
		

		numeric = tryAndGetDecimal(zeroFound);
		if (numeric != Token(Token::NONE)){
			return numeric;
		}

		return Token(Token::NONE);
	}

	bool isKeyWord(string buffer){
		if(keywords.count(buffer)){
			return true;
		}

	}

	Token getWord() {
		std::string buffer;

		while(Alphabet::isLetter(currentChar) || Alphabet::isDigit(currentChar)){
			buffer += currentChar;
			consume();
		}


		if (isKeyWord(buffer)){
			return Token(Token::KEYWORD, buffer);
		}

		return Token(Token::NAME, buffer, sourcePosition);

	}

	Token getDirective(){
		std::string buffer = "";
		while (!Alphabet::isNewline(currentChar) && !Alphabet::isEof(currentChar)){
			buffer += currentChar;
			consume();
		}
		return Token(Token::DIRECT, buffer, sourcePosition);
	}



	bool match (char c){
		if (currentChar == c)
			consume ();
		else
		    throw ParserException(std::string("Expected '") + c + std::string("' got '") + currentChar + std::string("'\n"));

	}



	void consume (){
		if (Alphabet::isNewline(currentChar)){
			sourcePosition.line ++;
			sourcePosition.linePosition = 1;
		}
		else {
			sourcePosition.linePosition++;
		}

		if (currentChar == EOF){
			throw ParserException("Unexpected end of file on " + sourcePosition.toString() + "\n");
		}
		
		currentPosition++; 
		
		
		
		if ( currentPosition >= (int)input.length() )
			currentChar = EOF;
		else {
			currentChar = input[currentPosition]; 
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

	bool find(std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != input[currentPosition + i])
				return false;
		}
		return true;
	}

	Token getMultyLineComment () {
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		while (!get("*/")){
			if (Alphabet::isEof(currentChar)){
				throw ParserException("Unfinished multiline comment on " + startPosition.toString());
			}
			buffer += currentChar;
			consume();
		}
		if (!Alphabet::isEof(currentChar)){
			consume();
		}
		return Token(Token::COMMENT, buffer);
	}

	Token getChar(){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		buffer += '\'';
		while (currentChar != '\''){
			if (Alphabet::isEof(currentChar)){
				throw ParserException("Unfinished charecter symbol on " + startPosition.toString());
			}
			buffer += currentChar;
			consume();		
		}
		consume();
		buffer += '\'';
		return Token(Token::CHAR, buffer); 
	}

	Token getComment (){
		std::string buffer;
		while (!Alphabet::isNewline(currentChar) && !Alphabet::isEof(currentChar)){
			buffer += currentChar;
			consume();
		}
		return Token(Token::COMMENT, buffer);
	}

	Token getString(){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		buffer += '\"';
		while (currentChar != '\"'){
			if (Alphabet::isEof(currentChar)){
				throw ParserException("Unfinished string on " + startPosition.toString());
			}
			buffer += currentChar;
			consume();
		}
		consume();
		buffer += '\"';
		return Token(Token::STRING, buffer); 
	}

	Token getSystemTokens(){

			while(Alphabet::isWhitespace(currentChar) || Alphabet::isNewline(currentChar) || Alphabet::isTab(currentChar)){
				consume();
			}

			if(Alphabet::isDigit(currentChar)){
				return tryAndGetNumeric();
			}
			else
			if (get("/*")){
				return  getMultyLineComment();
			}
			else
			if (Alphabet::isLetter(currentChar)){
				return  getWord();
			}
			else 
			if (get("/*")){
				return  getMultyLineComment();
			}
			else
			if(get("\'")){
				return getChar();
			}
			else
			if (get("//")){
				return  getComment();
			}
			else
			if(get("#")){
				return getDirective();
			}

			for(auto it: additionalOperators){
				if(find(it)){
					get(it);
					return Token(Token::OPERATOR, it);
				}
			}

			return Token(Token::NONE);
		

	}

	Token getDotVariants (){
		 if (currentChar == '.'){
			         	match ('.');
			         	if (currentChar == '.'){
			         		match('.');
			         		return Token(Token::OPERATOR, "...");
			         	}
			         	else {
			         		return Token(Token::OPERATOR, "..");
			         	}
			        }
			        else {
			         	return Token(Token::DOT, "." ); 
			     	}
	}

	Token getPlusVariants (){
		if ( currentChar == '+' ){
						match('+');
						return Token(Token::OPERATOR, "++"); 
					}
					else if ( currentChar == '=' ){
						match('=');
						return Token(Token::OPERATOR, "+=");
					}
					else {
						return Token(Token::OPERATOR, "+");
					}
	}


	Token getColonVariants(){
		if ( currentChar == '=' ) {
						match('=');
						return Token(Token::OPERATOR, ":="); 
					}
					else if (currentChar == ':'){
						match(':');
						if (currentChar == '='){
							match('=');
							return Token(Token::OPERATOR, "::=");
						}
						else {
							return Token(Token::OPERATOR, "::");
						}
					}
					else {
						return Token(Token::OPERATOR, ":");
					}
	}

	Token getMinusVariats(){
		if (currentChar == '>'){
						return Token(Token::OPERATOR, "->");
					}
					else if (currentChar == '-' ){
						match('-');
						if (currentChar == '>'){
							return Token(Token::OPERATOR, "-->");
						}
						else {
							return Token(Token::OPERATOR, "--");
						}
					}
					else if ( currentChar == '=' ){
						match('=');
						return Token(Token::OPERATOR, "-=");
					}
					else {
						return Token(Token::OPERATOR, "-");
					}
	}

	Token getEqualsVariants(){
		if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, "==");
					}
					else if (find(">>")){
						get(">>");
						return Token(Token::OPERATOR, "=>>");
					}
					else if (currentChar == '>'){
						match('>');
						return Token(Token::OPERATOR, "=>");
					}

					else {
						return Token(Token::OPERATOR, "=");
					}
	}

	Token getLessVariants(){
		if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, "<=");
					}
					else if (currentChar == '<'){
						match('<');
						return Token(Token::OPERATOR, "<<");
					}
					else {
						return Token(Token::OPERATOR, "<");

					}

	}

	Token getGreaterVariants(){
		if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, ">=");
					}
					else if (currentChar == '>'){
						match('>');
						if (currentChar == '>'){
							match('>');
							return Token(Token::OPERATOR, ">>>");
						}
						else {
							return Token(Token::OPERATOR, ">>");
						}
					}
					else {
						return Token(Token::OPERATOR, ">");

					}
	}

	Token getSlashVariants(){
			if (currentChar == '/'){
						match('/');
						return Token(Token::OPERATOR, "//");
					}
					else if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, "/=");
					}
					else {
						return Token(Token::OPERATOR, "/");
					}
	}

	Token getProcentVariants(){
			if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, "%%=");
					}
					else {
						return Token(Token::OPERATOR, "%%");
					}
	}

	Token getStarVariants(){
			if ( currentChar == '*' ){
						match('*');
						return Token(Token::OPERATOR, "**"); 
					}
					else if ( currentChar == '=' ){
						match('=');
						return Token(Token::OPERATOR, "*=");
					}
					else {
						return Token(Token::OPERATOR, "*");
					}

	}

	Token getAmpersandVariants(){
			if ( currentChar == '&' ){
				match('&');
				if (currentChar == '='){
					match('=');
					return Token(Token::OPERATOR, "&&="); 
				}
				return Token(Token::OPERATOR, "&&"); 
			}
			else if ( currentChar == '=' ){
				match('=');
				return Token(Token::OPERATOR, "&=");
			}
			else {
				return Token(Token::OPERATOR, "&");
			}

	}

	Token getDashVariants(){
			if (currentChar == '|' ){
				match('|');
				if (currentChar == '='){
					match('=');
					return Token(Token::OPERATOR, "||="); 
				}
				return Token(Token::OPERATOR, "||"); 
			}
			else if ( currentChar == '=' ){
				match('=');
				return Token(Token::OPERATOR, "|=");
			}
			else {
				return Token(Token::OPERATOR, "|");
			}

	}

	Token getWaveVariants(){
			if ( currentChar == '=' ){
				match('=');
				return Token(Token::OPERATOR, "~=");
			}
			else {
				return Token(Token::OPERATOR, "~");
			}

	}

	Token getExclamationMarkVariants(){
		if ( currentChar == '=' ){
				match('=');
				return Token(Token::OPERATOR, "!=");
			}
			else {
				return Token(Token::OPERATOR, "!");
			}
	}




	Token getSymbolicTokens(){

		switch (currentChar) {
					//case '#' : match('#'); return getDirective();
					case '(' : match('('); return Token(Token::BRACE_LEFT);
					case ')' : match(')'); return Token(Token::BRACE_RIGHT);
					case '[' : match('['); return Token(Token::BRACKET_LEFT);
					case ']' : match(']'); return Token(Token::BRACKET_RIGHT);
					case '{' : match('{'); return Token(Token::CURL_LEFT);
					case '}' : match('}'); return Token(Token::CURL_RIGHT);

			        case ',' : match(','); return Token(Token::COMMA);			       
			        case ';' : match(';'); return Token(Token::SEMICOLON);
			        case '?' : match('?'); return Token(Token::OPERATOR, "?");

			        case EOF: return Token (Token::END, "");

			        //case '\'': match ('\''); return getChar();
			        //case '\"': match ('\"'); return getString();

			        case '.' : match('.'); return getDotVariants();
					case ':' : match(':'); return getColonVariants();
					case '+' : match('+'); return getPlusVariants();				
					case '-' : match('-'); return getMinusVariats();									
					case '=' : match('='); return getEqualsVariants();		
					case '<' : match('<'); return getLessVariants();					
					case '>' : match('>'); return getGreaterVariants();				
					case '/' : match('/'); return getSlashVariants();
					case '%' : match('%'); return getProcentVariants();
					case '*' : match('*'); return getStarVariants();
					case '&' : match('&'); return getAmpersandVariants();
					case '|' : match('|'); return getDashVariants();
					case '~' : match('~'); return getWaveVariants();
					case '!' : match('!'); return getExclamationMarkVariants();
				default: 
					if (Alphabet::isSpecialSymbol(currentChar)){
						char c = currentChar; 
						consume();
						return Token(Token::OPERATOR, std::string("") + c);
					}
					else{
						if (!Alphabet::isAcceptableCharacter(currentChar)){
							throw ParserException(std::string("Unknown character ") + currentChar + "on " 
								+ sourcePosition.toString() + "\n" );
						}
					}

			}

			return Token(Token::NONE);

	}
	Token getNextToken(){
		while (currentChar != EOF){
		
			Token currentToken;
			currentToken = getSystemTokens();
			if (currentToken != Token(Token::NONE)){
				return currentToken;
			}

			currentToken = getSymbolicTokens();
			if (currentToken != Token(Token::NONE)){
				return currentToken;
			}


			
		}
		return Token (Token::END, "", sourcePosition);

	}

	void parseDirective(string text){
		stringstream ss(text);

		string cur;

		ss >> cur;

		if(cur == "keyword"){
			while(ss >> cur){
				keywords.insert(cur);
			}
		}

		if(cur == "operator"){
			while(ss >> cur){
				additionalOperators.push_back(cur);
			}
		}

	}

	void tokenize (){
		Token currentToken;
		currentToken = Token(Token::BEGIN, "", Token::Position());
		do {
			output.push_back(currentToken);
			currentToken = getNextToken();
			if(currentToken.type == Token::DIRECT){
				parseDirective(currentToken.text);
			}
		}
		while (currentToken != Token(Token::END, ""));
		//std::cout << "kk";
		output.push_back(currentToken);
	}


};
