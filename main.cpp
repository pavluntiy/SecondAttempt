#include "lexer.h"
#include "preproc.h"
#include <iostream>
#include <fstream>

int main (){
	ifstream in ("input.in");
	ofstream out ("output.out");
	ofstream parsed ("parsed.out");
	string input = "";
	while (!in.eof()) { 
		input += in.get(); 
	}


	//Lexer lexer(input); 
	Preprocessor pr(input);
	try{
		pr.preprocess();
	//	lexer.tokenize();
	}
	catch (PreprocessorException pe){
		cout << pe.what();
	}

	Lexer lexer(pr.output); 
	try{
		lexer.tokenize();
	}
	catch(ParserException pe){
		cout << pe.what();
	}

	out << pr.output;

	for(auto cc: lexer.output){
		parsed << cc;
	}
	return 0;
}
