#ifndef LEXER_H
#define LEXER_H
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
	Position sourcePosition;


	Lexer(string input){
		this->sourcePosition = Position(1, 1);
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
		Position startPosition = sourcePosition;
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
				return makeToken(Token::FLOAT, buffer);
			}
			return makeToken(Token::INT, buffer);
		}

		return makeToken(Token::NONE);
	}

	Token tryAndGetBinar(bool zeroFound){
		Position startPosition = sourcePosition;
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
				return makeToken(Token::FLOAT, buffer, startPosition);
			}
			return makeToken(Token::INT, buffer, startPosition);
		}

		return makeToken(Token::NONE);
	}

	Token tryAndGetOctal(bool zeroFound){
		Position startPosition = sourcePosition;
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
				return makeToken(Token::INT, "0", sourcePosition);
			}
			buffer += std::string(" (octal)");



			if(Alphabet::isDigit(currentChar)){
				throw ParserException("Nonoctal digits in octal number " + sourcePosition.toString());
			}
			if (isFloat){
				return makeToken(Token::FLOAT, buffer, startPosition);
			}
			return makeToken(Token::INT, buffer, startPosition);
		}

		return makeToken(Token::NONE);
	}


	Token tryAndGetDecimal(bool zeroFound){
		Position startPosition = sourcePosition;
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
				return makeToken(Token::FLOAT, buffer, startPosition);
			}
			return makeToken(Token::INT, buffer, startPosition);

		}

		return makeToken(Token::NONE);
	}


	Token tryAndGetNumeric(){
		Token numeric;
		bool zeroFound = false;

		if (currentChar == '0'){
			zeroFound = true;
			consume();
		}

		numeric = tryAndGetHexadecimal(zeroFound);
		if (numeric != makeToken(Token::NONE)){
			return numeric;
		}

		numeric = tryAndGetBinar(zeroFound);
		if (numeric != makeToken(Token::NONE)){
			return numeric;
		}

		numeric = tryAndGetOctal(zeroFound);
		if (numeric != makeToken(Token::NONE)){
			return numeric;
		}
		

		numeric = tryAndGetDecimal(zeroFound);
		if (numeric != makeToken(Token::NONE)){
			return numeric;
		}

		return makeToken(Token::NONE);
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
			return makeToken(Token::KEYWORD, buffer);
		}

		return makeToken(Token::NAME, buffer, sourcePosition);

	}

	Token getDirective(){
		std::string buffer = "";
		while (!Alphabet::isNewline(currentChar) && !Alphabet::isEof(currentChar) && !find("//") && !find("/*")){
			buffer += currentChar;
			consume();
		}

		cout << currentChar << '\n';
		return makeToken(Token::DIRECT, buffer, sourcePosition);
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
		Position startPosition = sourcePosition;
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
		return makeToken(Token::COMMENT, buffer);
	}

	Token getChar(){
		Position startPosition = sourcePosition;
		std::string buffer;
		buffer += '\'';
		bool wasSlash = false;
		while (currentChar != '\'' || wasSlash && currentChar == '\''){
			wasSlash = false;
			if (Alphabet::isEof(currentChar)){
				
				throw ParserException("Unfinished character symbol on " + startPosition.toString());
			}

			if(currentChar == '\\'){
				wasSlash = true;
			}
			buffer += currentChar;
			consume();		
		}
		consume();
		buffer += '\'';
		return makeToken(Token::CHAR, buffer); 
	}

	Token getComment (){
		std::string buffer;
		while (!Alphabet::isNewline(currentChar) && !Alphabet::isEof(currentChar)){
			buffer += currentChar;
			consume();
		}
		return makeToken(Token::COMMENT, buffer);
	}

	Token getString(){
		Position startPosition = sourcePosition;
		std::string buffer;
		buffer += '\"';
		bool wasSlash = false;
		while (currentChar != '\"' || wasSlash && currentChar == '\"'){
			wasSlash = false;
			if (Alphabet::isEof(currentChar)){
				throw ParserException("Unfinished string on " + startPosition.toString());
			}

			if(currentChar == '\\'){
				wasSlash = true;
			}
			buffer += currentChar;
			consume();
		}
		consume();
		buffer += '\"';
		return makeToken(Token::STRING, buffer); 
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
			else
			for(auto it: additionalOperators){
				if(find(it)){
					get(it);
					return makeToken(Token::OPERATOR, it);
				}
			}

			return makeToken(Token::NONE);
		

	}

	Token getDotVariants (){
		 if (currentChar == '.'){
			         	match ('.');
			         	if (currentChar == '.'){
			         		match('.');
			         		return makeToken(Token::OPERATOR, "...");
			         	}
			         	else {
			         		return makeToken(Token::OPERATOR, "..");
			         	}
			        }
			        else {
			         	return makeToken(Token::DOT, "." ); 
			     	}
	}

	Token getPlusVariants (){
		if ( currentChar == '+' ){
						match('+');
						return makeToken(Token::OPERATOR, "++"); 
					}
					else if ( currentChar == '=' ){
						match('=');
						return makeToken(Token::OPERATOR, "+=");
					}
					else {
						return makeToken(Token::OPERATOR, "+");
					}
	}


	Token getColonVariants(){
		if ( currentChar == '=' ) {
						match('=');
						return makeToken(Token::OPERATOR, ":="); 
					}
					else if (currentChar == ':'){
						match(':');
						if (currentChar == '='){
							match('=');
							return makeToken(Token::OPERATOR, "::=");
						}
						else {
							return makeToken(Token::OPERATOR, "::");
						}
					}
					else {
						return makeToken(Token::OPERATOR, ":");
					}
	}

	Token getMinusVariats(){
		if (currentChar == '>'){
						return makeToken(Token::OPERATOR, "->");
					}
					else if (currentChar == '-' ){
						match('-');
						if (currentChar == '>'){
							return makeToken(Token::OPERATOR, "-->");
						}
						else {
							return makeToken(Token::OPERATOR, "--");
						}
					}
					else if ( currentChar == '=' ){
						match('=');
						return makeToken(Token::OPERATOR, "-=");
					}
					else {
						return makeToken(Token::OPERATOR, "-");
					}
	}

	Token getEqualsVariants(){
		if (currentChar == '='){
						match('=');
						return makeToken(Token::OPERATOR, "==");
					}
					else if (find(">>")){
						get(">>");
						return makeToken(Token::OPERATOR, "=>>");
					}
					else if (currentChar == '>'){
						match('>');
						return makeToken(Token::OPERATOR, "=>");
					}

					else {
						return makeToken(Token::OPERATOR, "=");
					}
	}

	Token getLessVariants(){
		if (currentChar == '='){
						match('=');
						return makeToken(Token::OPERATOR, "<=");
					}
					else if (currentChar == '<'){
						match('<');
						return makeToken(Token::OPERATOR, "<<");
					}
					else {
						return makeToken(Token::OPERATOR, "<");

					}

	}

	Token getGreaterVariants(){
		if (currentChar == '='){
						match('=');
						return makeToken(Token::OPERATOR, ">=");
					}
					else if (currentChar == '>'){
						match('>');
						if (currentChar == '>'){
							match('>');
							return makeToken(Token::OPERATOR, ">>>");
						}
						else {
							return makeToken(Token::OPERATOR, ">>");
						}
					}
					else {
						return makeToken(Token::OPERATOR, ">");

					}
	}

	Token getSlashVariants(){
			if (currentChar == '/'){
						match('/');
						return makeToken(Token::OPERATOR, "//");
					}
					else if (currentChar == '='){
						match('=');
						return makeToken(Token::OPERATOR, "/=");
					}
					else {
						return makeToken(Token::OPERATOR, "/");
					}
	}

	Token getProcentVariants(){
			if (currentChar == '='){
						match('=');
						return makeToken(Token::OPERATOR, "%%=");
					}
					else {
						return makeToken(Token::OPERATOR, "%%");
					}
	}

	Token getStarVariants(){
			if ( currentChar == '*' ){
						match('*');
						return makeToken(Token::OPERATOR, "**"); 
					}
					else if ( currentChar == '=' ){
						match('=');
						return makeToken(Token::OPERATOR, "*=");
					}
					else {
						return makeToken(Token::OPERATOR, "*");
					}

	}

	Token getAmpersandVariants(){
			if ( currentChar == '&' ){
				match('&');
				if (currentChar == '='){
					match('=');
					return makeToken(Token::OPERATOR, "&&="); 
				}
				return makeToken(Token::OPERATOR, "&&"); 
			}
			else if ( currentChar == '=' ){
				match('=');
				return makeToken(Token::OPERATOR, "&=");
			}
			else {
				return makeToken(Token::OPERATOR, "&");
			}

	}

	Token getDashVariants(){
			if (currentChar == '|' ){
				match('|');
				if (currentChar == '='){
					match('=');
					return makeToken(Token::OPERATOR, "||="); 
				}
				return makeToken(Token::OPERATOR, "||"); 
			}
			else if ( currentChar == '=' ){
				match('=');
				return makeToken(Token::OPERATOR, "|=");
			}
			else {
				return makeToken(Token::OPERATOR, "|");
			}

	}

	Token getWaveVariants(){
			if ( currentChar == '=' ){
				match('=');
				return makeToken(Token::OPERATOR, "~=");
			}
			else {
				return makeToken(Token::OPERATOR, "~");
			}

	}

	Token getExclamationMarkVariants(){
		if ( currentChar == '=' ){
				match('=');
				return makeToken(Token::OPERATOR, "!=");
			}
			else {
				return makeToken(Token::OPERATOR, "!");
			}
	}




	Token getSymbolicTokens(){

		switch (currentChar) {
					//case '#' : match('#'); return getDirective();
					case '(' : match('('); return makeToken(Token::BRACE_LEFT);
					case ')' : match(')'); return makeToken(Token::BRACE_RIGHT);
					case '[' : match('['); return makeToken(Token::BRACKET_LEFT);
					case ']' : match(']'); return makeToken(Token::BRACKET_RIGHT);
					case '{' : match('{'); return makeToken(Token::CURL_LEFT);
					case '}' : match('}'); return makeToken(Token::CURL_RIGHT);

			        case ',' : match(','); return makeToken(Token::COMMA);			       
			        case ';' : match(';'); return makeToken(Token::SEMICOLON);
			        case '?' : match('?'); return makeToken(Token::OPERATOR, "?");

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
						return makeToken(Token::OPERATOR, std::string("") + c);
					}
					else{
						if (!Alphabet::isAcceptableCharacter(currentChar)){
							throw ParserException(std::string("Unknown character ") + currentChar + "on " 
								+ sourcePosition.toString() + "\n" );
						}
					}

			}

			return makeToken(Token::NONE);

	}
	Token getNextToken(){
		while (currentChar != EOF){
		
			Token currentToken;
			currentToken = getSystemTokens();
			if (currentToken != makeToken(Token::NONE)){
				return currentToken;
			}

			currentToken = getSymbolicTokens();
			if (currentToken != makeToken(Token::NONE)){
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
		currentToken = Token(Token::BEGIN, "", Position());
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

	Token makeToken(Token::Type type = Token::NONE, string text = "", Position position = {0, 0}){
		if(position.line == 0 && position.linePosition == 0){
			return Token(type, text, sourcePosition);
		}
		return Token(type, text, position);
	}


};

#endif
