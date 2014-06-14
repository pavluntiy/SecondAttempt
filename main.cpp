#include "lexer.h"
#include "preproc.h"
#include <iostream>
#include <fstream>

int main (){
	ifstream in ("input.in");
	ofstream out ("output.out");
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
//	catch (ParserException pe){
//		cout << pe.what();
//	}

	for(auto cc: pr.directives){
		out << cc.text << '\n';
	}

//	for(auto cc: lexer.output){
//		out << cc;
//	}
	return 0;
}
