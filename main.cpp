#include "lexer.h"
#include <iostream>
#include <fstream>

int main (){
	ifstream in ("input.in");
	ofstream out ("output.out");
	string input = "";
	while (!in.eof()) { 
		input += in.get(); 
	}


	Lexer lexer(input); 
	try{
		lexer.tokenize();
	}
	catch (ParserException pe){
		cout << pe.what();
	}

	for(auto cc: lexer.output){
		out << cc;
	}
	return 0;
}
