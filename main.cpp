#include "lexer.h"
#include "recognitionexception.h"
#include "preproc.h"
#include <iostream>
#include <fstream>

#include "parser.h"



ofstream treeOut;

void dfs (Node *node, string shift = ""){
	treeOut << shift << "( " << node->typeToText();
	if(node->text != ""){
	 	treeOut<< " \"" << node->text << "\"";
	}
	if(node->children.size() > 0){
	 	treeOut << ", children num = "<< node->children.size() << ":\n";
	}
		for(int i = 0; i < node->children.size(); ++i){
			dfs(node->children[i], shift + ' ');
		}
	if(node->children.size() > 0){
	 	treeOut << shift;
	}
	else {
		treeOut << ' ';
	}
	treeOut   << ")\n";
	
}
void draw(Node *tree, string str){
	treeOut.open(str);

	dfs(tree);
}

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
		cout << pe.what()<< "\n";
		cout << "Process terminated\n";
		return 0;
	}

	Lexer lexer(pr.output); 
	try{
		lexer.tokenize();
	}
	catch(ParserException pe){
		cout << pe.what() << "\n";
		cout << "Process terminated\n";
		return 0;
	}

	out << pr.output;

	for(auto cc: lexer.output){
		parsed << cc;
	}

	Parser parser(lexer.output);

	try{
		parser.buildTree();
	}
	catch (RecognitionException re){
		cout << re.what()<< "\n";
		cout << "Process terminated\n";
		return 0;
	} 
//	cout << "asdfasdfasdfasd\n";
	draw(parser.tree, "tree.out");
	return 0;
}
