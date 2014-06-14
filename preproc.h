#ifndef PREPROC_H
#define PREPROC_H
#include "preprocexception.h"
#include "log.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "alphabet.h"

#include <string>
#include <vector>

class Preprocessor {
public:
	class Directive {
	public:
		string text;
		Token::Position position;

		Directive(string text = "", Token::Position position = {0, 0}){
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
	Token::Position sourcePosition;

	ifstream in;

	Preprocessor (string input){
		this->input = input;
		this->currentPosition = 0;
		this->currentChar = input[currentPosition];
		this->output = "";
	}

	~Preprocessor(){

	}
	
	
	Directive getDirective(){
		std::string buffer = "";
		while (!Alphabet::isNewline(currentChar) && !Alphabet::isEof(currentChar)){
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

		if (currentChar == EOF){
			throw PreprocessorException("Unexpected end of file on " + sourcePosition.toString() + "\n");
		}
		
		currentPosition++; 
		
		
		
		if ( currentPosition >= (int)input.length() )
			currentChar = EOF;
		else {
			currentChar = input[currentPosition]; 
		}
	}

	void preprocess(){
		Directive currentDirective;
		while (currentChar != EOF){
			
			if(currentChar == '#'){
				consume();
				currentDirective = getDirective();
				directives.push_back(currentDirective);
				cout << directives.size();
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