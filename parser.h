#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include "recognitionexception.h"
#include <fstream>

#include "node.h"

/*
	I believe that 'recoil()' after each clause is necessary, because 'match()' consumes tokens.
*/


const bool ignore = true;

ofstream log("log.out");
class Parser {
public:

	int depth;
	bool logDepth;
	vector<Token> input;
	Node *tree;
	int currentPosition, previousPosition;
	Token currentToken;
	map<pair<Node::NodeType, int>, pair<bool, int> > memo;

	void recoil(int previousPosition = 0){
		log << "\t=>Recoiled from " << currentPosition << " to " << previousPosition << ", current token = ";		
		if(previousPosition >= 0 && previousPosition < input.size()){
			currentPosition = previousPosition;
		}


		currentToken = input[currentPosition];
		log << currentToken.typeToText() << "\n";
	}

	void consume(bool stop = false){
		if(stop){
			return;
		}
		currentPosition++;
		if(currentPosition < input.size())
				currentToken = input[currentPosition];
			else{
				currentPosition--;
				currentToken = Token(Token::END);
			}
		while(currentToken == Token(Token::BEGIN) ||
			currentToken == Token(Token::COMMENT) || currentToken == Token(Token::DIRECT)
		){
			currentPosition++;
			if(currentPosition < input.size())
				currentToken = input[currentPosition];
			else{
				currentPosition--;
				currentToken = Token(Token::END);
			}
		}
	}

bool match(Token::Type type){
	log << "Looking for " << Token(type).typeToText() << " on position " << currentPosition;
	depth++;
	if(logDepth){
		log <<" on depth = " << depth;
	}
	log << "\n";
	if(type == currentToken.type){
		log << "\t Found " << currentToken.typeToText()<< "\n";
		consume();
		depth--;
		return true;
	}
	else {
		if(type == Token::CURL_RIGHT || type == Token::BRACE_RIGHT){
			log << "---!WARNING!---\n";
			log << ("\t\tYou could have missed " + typeToText(type) + " on position " + currentToken.position.toString() + " (Token # " + std::to_string(currentPosition) +")");
		}
		depth--;
		log << "\t Failed to find " << Token(type).typeToText() << ", got " << currentToken.typeToText() << " on position " << currentPosition << "\n";
		return false;
	}
}
bool match(Token token){
	depth++;
	log << "Looking for " << token.typeToText() << " \"" <<token.getText() << " \" on position " << currentPosition;
	if(logDepth){
		log <<" on depth = " << depth;
	}
	log << "\n";
	if(currentToken.type == token.type && currentToken.text == token.text){
		log << "\t Found "<< token.typeToText() << " \"" <<token.getText() << " \" on position " << currentPosition << "\n";
		consume();
		depth--;
		return true;
	}


	if(token.text == "" && currentToken.type == token.type){
		depth--;
		consume();
		return true;
	}

	depth--;
	log << "\t Failed to find " << token.typeToText() << ", \"" << token.getText() << "\", got " << currentToken.typeToText() << ", \"" << currentToken.getText()<< " \" on position " << currentPosition << "\n";
	return false;
}

	bool match(Node::NodeType what){
		depth++;
		log << "Looking for " << Node(what).typeToText() << " on position "  << currentPosition;
		if(logDepth){
			log <<" on depth = " << depth;
		}
		log << "\n";

		
		
		log.flush();
		int previousPosition = currentPosition;
		bool result = false;
		if(memo.count(make_pair(what, currentPosition))){	
			result =  memo[make_pair(what, currentPosition)].first;
			log << "\t\tGot in memo: " << Node(what).typeToText() << " on position "  << currentPosition << " is " << result << "\n";
			if(result){
				log << "\t\t\tFrom position " << currentPosition << " and token " << currentToken.typeToText() << ", " << currentToken.getText() << " to \n ";
				recoil(memo[make_pair(what, currentPosition)].second);
				log << "\t\t\t\tnew position " << currentPosition << "and new token " << currentToken.typeToText() << ", " << currentToken.getText()  << "\n";
			}
			
			
		}
		else{
			switch(what){
/*CHANGED*/
				case Node::VALUE: {				
									/*			if(match(Node::NAME) && match(Token::BRACKET_LEFT) && match(Node::EXPRESSION) && match(Token::BRACKET_RIGHT)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL) && match(Token::BRACKET_LEFT) && match(Node::EXPRESSION) && match(Token::BRACKET_RIGHT)){
													result = true;
													break;
												}
												else recoil(previousPosition);
									*/
												if(match(Node::ARGLIST)){
													result = true;
													break;
												}
												else recoil(previousPosition);

									/*			if(match(Node::FUNCALL) && match(Token::DOT) && match(Node::VALUE)){
													result = true;
													break;
												}
												else recoil(previousPosition);
									*/
									/*			if(match(Node::FUNCALL) && match(Token(Token::OPERATOR, "->")) && match(Node::VALUE)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL)){
													result = true;
													break;
												}
												else recoil(previousPosition);
									*/
												if(match(Token::INT)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::FLOAT)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::BOOL)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::CHAR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::STRING)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::NAME)){
													result = true;
													break;
												}
												else recoil(previousPosition);


												
											} break;

/*CHANGED*/
				case Node::BRACED: 			{
												if(match(Node::ARGLIST) && match(Node::BRACED)){
													result = true;
													break;
												}
												else recoil(previousPosition);
												if(match(Node::ACCESS) && match(Node::BRACED)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::ARGLIST)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::ACCESS)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											
											} break;
/*CHANGED*/
				case Node::ACCESS: 			{										
												if(match(Token::BRACKET_LEFT) && match(Node::EXPRESSION) && match(Token::BRACKET_RIGHT)){
													result = true;
													break;
												}
												else recoil(previousPosition);

											} break;


				case Node::ACCESS_EXPR:      {
												if(match(Node::EXPR8) && match(Node::ACCESS)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											}
/*CHANGED*/
				case Node::EXPR8: 			{
												if(match(Token::BRACE_LEFT) && match(Node::EXPRESSION) && match(Token::BRACE_RIGHT)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if (match(Node::VALUE)) {
													result = true;
													break;
												}
												else recoil(previousPosition);
											
											} break;
/*CHANGED*/
				case Node::EXPR7: 			{										
												if(match(Node::FUNCALL) && match(Node::BRACED)){

													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::ACCESS_EXPR) && match(Node::BRACED)){

													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL)){

													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::ACCESS_EXPR)){

													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR8)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;	
/*CHANGED*/
			case Node::EXPR6: 			{
												if(match(Node::EXPR7) && match(Token(Token::OPERATOR, "->" )) && match(Node::EXPR6)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR7) && match(Token::DOT) && match(Node::EXPR6)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token(Token::OPERATOR, "*" )) && match(Node::EXPR7)){
													result = true;
													break;
												}
												else recoil(previousPosition);

					/*							if(match(Node::EXPR7) && match(Node::ARGLIST) && match(Token(Token::OPERATOR, "->")) && match(Node::EXPR6)){
													result = true;
													break;
												}
												else recoil(previousPosition);
						*/
												if(match(Node::EXPR7)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											
											} break;

				case Node::EXPR5: 			{										
												if(match(Node::EXPR6) && match(Token(Token::OPERATOR, "**")) && match(Node::EXPR5)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR6)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;	

				case Node::EXPR4: 			{
												if(match(Token(Token::OPERATOR, "~")) && match(Node::EXPR5)){
													result = true;
													break;
												}
												else recoil(previousPosition);
												if (match(Token(Token::OPERATOR, "!")) && match(Node::EXPR5)) {
													result = true;
													break;
												}
												else recoil(previousPosition);
												if(match(Token(Token::OPERATOR, "-")) && match(Node::EXPR5)){
													result = true;
													break;
												}
												else recoil(previousPosition);
												if(match(Token(Token::OPERATOR, "+")) && match(Node::EXPR5)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token(Token::OPERATOR, "+")) && match(Node::EXPR5)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR5)){
													result = true;
													break;
												}
												else recoil(previousPosition);


											} break;

				

				case Node::EXPR3: 			{										
												if(match(Node::EXPR4) && match(Token(Token::OPERATOR, "*")) && match(Node::EXPR3)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR4) && match(Token(Token::OPERATOR, "/")) && match(Node::EXPR3)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR4) && match(Token(Token::OPERATOR, "%")) && match(Node::EXPR3)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR4)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;	
					case Node::EXPR2: 			{										
												if(match(Node::EXPR3) && match(Token(Token::OPERATOR, "+")) && match(Node::EXPR2)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR3) && match(Token(Token::OPERATOR, "-")) && match(Node::EXPR2)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR3)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;	

					case Node::EXPR1: 			{										
												if(match(Node::EXPR2) && match(Token(Token::OPERATOR, "<<")) && match(Node::EXPR1)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR2) && match(Token(Token::OPERATOR, ">>")) && match(Node::EXPR1)){
													result = true;
													break;
												}
												else recoil(previousPosition);


												if(match(Node::EXPR2)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;	

					case Node::B_AND: 			{										
												if(match(Node::EXPR1) && match(Token(Token::OPERATOR, "&")) && match(Node::B_AND)){
													result = true;
													break;
												}
												else recoil(previousPosition);


												if(match(Node::EXPR1)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;

					case Node::B_XOR: 			{										
												if(match(Node::B_AND) && match(Token(Token::OPERATOR, "^")) && match(Node::B_XOR)){
													result = true;
													break;
												}
												else recoil(previousPosition);


												if(match(Node::B_AND)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;

					case Node::B_OR: 			{										
												if(match(Node::B_XOR) && match(Token(Token::OPERATOR, "|")) && match(Node::B_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);


												if(match(Node::B_XOR)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;

				case Node::COMPARISION: {										
												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "==")) && match(Node::COMPARISION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, ">")) && match(Node::COMPARISION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "<")) && match(Node::COMPARISION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, ">=")) && match(Node::COMPARISION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "<=")) && match(Node::COMPARISION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "!=")) && match(Node::COMPARISION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

											} break;


						case Node::IS_IN_EXPRESSION: {
										if(match(Node::COMPARISION) && match(Token(Token::KEYWORD, "is")) && match(Node::IS_IN_EXPRESSION)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::COMPARISION) && match(Token(Token::KEYWORD, "in")) && match(Node::IS_IN_EXPRESSION)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::COMPARISION)){
													result = true;
													break;
										}
										else recoil(previousPosition);


									} break;

					case Node::L_AND: 			{										
												if(match(Node::IS_IN_EXPRESSION) && match(Token(Token::OPERATOR, "&&")) && match(Node::L_AND)){
													result = true;
													break;
												}
												else recoil(previousPosition);


												if(match(Node::IS_IN_EXPRESSION)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;

					case Node::L_OR: 			{										
												if(match(Node::L_AND) && match(Token(Token::OPERATOR, "||")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);


												if(match(Node::L_AND)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;



					case Node::EXPRESSION: {			
							
												if(match(Node::ASSIGNMENT)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);
									
							
												

												

												
											} break;

					case Node::ASSIGNMENT: {		

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "=")) && match(Node::EXPRESSION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "+=")) && match(Node::EXPRESSION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "-=")) && match(Node::EXPRESSION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::NAME) && match(Token(Token::OPERATOR, "/=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::NAME) && match(Token(Token::OPERATOR, "%=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::NAME) && match(Token(Token::OPERATOR, "*=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::NAME) && match(Token(Token::OPERATOR, "|=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::NAME) && match(Token(Token::OPERATOR, "||=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::NAME) && match(Token(Token::OPERATOR, "&=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::NAME) && match(Token(Token::OPERATOR, "&&=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

											} break;




					case Node::FUNCALL: {


										if(match(Node::EXPR8) && match(Node::ARGLIST)){
													result = true;
													break;
										}
										else recoil(previousPosition);



										} break;


					case Node::NAME: {

										if(match(Node::NAMESPACE) && match(Token(Token::OPERATOR, "::")) && match(Node::NAME)){
													result = true;
												break;
											}
										else recoil(previousPosition);

							/*			if(match(Token(Token::OPERATOR, "*")) && match(Node::NAME1)){
													result = true;
												break;
											}
										else recoil(previousPosition);

										if(match(Node::NAME1) && match(Token(Token::OPERATOR, "->")) && match(Node::NAME)){
													result = true;
												break;
											}
										else recoil(previousPosition);

										if(match(Node::NAME1) && match(Token(Token::DOT, ".")) && match(Node::NAME)){
													result = true;
													break;
											}
											else recoil(previousPosition);
						*/
										if(match(Node::NAME1)){
													result = true;
												break;
											}
										else recoil(previousPosition);






										} break;

					case Node::NAME1: {	


											if(match(Token::NAME)){
													result = true;
												break;
											}
											else recoil(previousPosition);

											if(match(Token::BRACE_LEFT) && match(Node::NAME) && match(Token::BRACE_RIGHT)){
													result = true;
												break;
											}
											else recoil(previousPosition);

											

										} break;
					case Node::NAMESPACE: {
											if(match(Token::NAME)){
													result = true;
													break;
											}
											else recoil(previousPosition);

										} break;

					case Node::LIST: {	
										

										if(match(Node::LIST_ELEM) && match(Token::COMMA) && match(Node::LIST)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::LIST_ELEM)){
													result = true;
													break;
										}
										else recoil(previousPosition);


									} break;		
					case Node::LIST_ELEM: {	
										

										if(match(Token::BRACE_LEFT) && match(Node::LIST) && match(Token::BRACE_RIGHT)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::ASSIGNMENT)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::L_OR)){
													result = true;
													break;
										}
										else recoil(previousPosition);


										

										if(match(Node::VALUE)){
													result = true;
													break;
										}
										else recoil(previousPosition);


									} break;	

					case Node::ARGLIST: {
									if(match(Token::BRACE_LEFT) && match(Node::LIST) && match(Token::BRACE_RIGHT)){
													result = true;
													break;
										}
										else recoil(previousPosition);
									if(match(Token::BRACE_LEFT) && match(Token::BRACE_RIGHT)){
													result = true;
													break;
										}
									else recoil(previousPosition);

									} break;

				

					case Node::OPERATOR: {
										if(match(Node::SPECIAL)){
													result = true;
													break;
										}
										else recoil(previousPosition);
										if(match(Node::FUNCDEF)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::DECLARATION) && match(Token::SEMICOLON)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::EXPRESSION) && match(Token::SEMICOLON)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::BODY)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Token::SEMICOLON)){
													result = true;
													break;
										}
										else recoil(previousPosition);



									} break;

				case Node::OPERATORS: {
										if(match(Node::OPERATOR) &&  match(Node::OPERATORS)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(true){
													result = true;
													break;
										}
										else recoil(previousPosition);


									} break;				

				case Node::BODY: {
										if(match(Token::CURL_LEFT) &&  match(Node::OPERATORS) && match(Token::CURL_RIGHT)){
													result = true;
													break;
										}
										else recoil(previousPosition);


									} break;

				case Node::DECLARATION: {
										if(match(Node::TYPE) &&  match(Node::VARLIST)) {
													result = true;
													break;
										}
										else recoil(previousPosition);


									} break;
				case Node::VARLIST: {	


										if(match(Node::ASSIGNMENT) && match(Token::COMMA) && match(Node::VARLIST)) {
													result = true;
													break;
										}
										else recoil(previousPosition);



										if(match(Node::ASSIGNMENT)) {
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::NAME) && match(Token::COMMA) && match(Node::VARLIST)) {
													result = true;
													break;
										}
										else recoil(previousPosition);



										if(match(Node::NAME)) {
													result = true;
													break;
										}
										else recoil(previousPosition);

									} break;			

				case Node::TYPE: {
										if(match(Node::NAME)) {
													result = true;
													break;
										}
										else recoil(previousPosition);


									} break;

				case Node::FUNCDEF: {
										if(match(Node::TYPE) && match(Node::NAME) && match(Token::BRACE_LEFT) && match(Node::DECL_LIST) && match(Token::BRACE_RIGHT) && match(Node::BODY)) {
													result = true;
													break;
										}
										else recoil(previousPosition);


									} break;
				case Node::DECL_LIST: {
										if(match(Node::DECL_ATOM) && match(Token::COMMA) && match(Node::DECL_LIST)) {
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::DECL_ATOM)) {
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(true) {
													result = true;
													break;
										}
										else recoil(previousPosition);


									} break;
				case Node::DECL_ATOM: {
										if(match(Node::TYPE) && match(Node::ASSIGNMENT)) {
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::TYPE) && match(Node::NAME)) {
													result = true;
													break;
										}
										else recoil(previousPosition);


									} break;

				case Node::FORTHING: {	
										if(match(Node::DECLARATION) ){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::EXPRESSION)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										
										

										if(match(Node::BODY)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(true){
													result = true;
													break;
										}
										else recoil(previousPosition);

									} break;
				case Node::SPECIAL: {

								if(match(Token(Token::KEYWORD, "foreach")) && match(Token::BRACE_LEFT) &&
										match(Node::DECLARATION) && match(Token(Token::OPERATOR, ":")) && match(Node::EXPRESSION)&&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) 
								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(match(Token(Token::KEYWORD, "for")) && match(Token::BRACE_LEFT) &&
										match(Node::FORTHING) && match(Token::SEMICOLON) && 
										match(Node::FORTHING) && match(Token::SEMICOLON) && 
										match(Node::FORTHING) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) 
								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "if")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(match(Token(Token::KEYWORD, "if")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) 
								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "finally")) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);


								if(		match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "finally")) && match(Node::OPERATOR)

								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);



								if(match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) 
								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "init")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "finally")) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);


								if(		match(Token(Token::KEYWORD, "init")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "init")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "finally")) && match(Node::OPERATOR)

								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);



								if(		match(Token(Token::KEYWORD, "init")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) 
								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);


								if(		match(Token(Token::KEYWORD, "do")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) &&
								 		match(Token::SEMICOLON)
								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "switch")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) &&
								 		match(Token::CURL_LEFT) && match(Node::CASES) && match(Token::CURL_RIGHT) &&
								 		match(Token(Token::KEYWORD, "finally")) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)
								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "switch")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) &&
								 		match(Token::CURL_LEFT) && match(Node::CASES) && match(Token::CURL_RIGHT) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)
								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);


								if(		match(Token(Token::KEYWORD, "switch")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) &&
								 		match(Token::CURL_LEFT) && match(Node::CASES) && match(Token::CURL_RIGHT)

								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(	
										match(Token(Token::KEYWORD, "return")) && 
										match(Node::EXPRESSION) &&
								 		match(Token::SEMICOLON)

								 ) {
													result = true;
													break;
										}
								else recoil(previousPosition);

								if(	
										match(Token(Token::KEYWORD, "return")) && 
								 		match(Token::SEMICOLON)

								 ) {
													result = true;
													break;
										}
								else recoil(previousPosition);

								if(	
										match(Token(Token::KEYWORD, "break")) && 
										match(Node::NAME1) &&
								 		match(Token::SEMICOLON)

								 ) {
													result = true;
													break;
										}
								else recoil(previousPosition);

								if(	
										match(Token(Token::KEYWORD, "break")) && 
								 		match(Token::SEMICOLON)

								 ) {
													result = true;
													break;
										}
								else recoil(previousPosition);

								if(	
										match(Token(Token::KEYWORD, "goto")) && 
										match(Node::NAME1) &&
								 		match(Token::SEMICOLON)

								 ) {
													result = true;
													break;
										}
								else recoil(previousPosition);



								} break;


				case Node::CASES: {
										if(match(Node::CASE) && match(Node::CASES)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::CASE)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(true){
											result = true;
													break;
										}
										else recoil(previousPosition);


				} break;

				case Node::CASE: {
						if(
							match(Token(Token::KEYWORD, "case")) && match(Node::EXPRESSION) && 
							match(Token(Token::OPERATOR, ":")) &&
							match(Node::OPERATOR) &&
							match(Token(Token::KEYWORD, "continue")) &&  match(Token::SEMICOLON)
						){
									result = true;
									break;
						}
						else recoil(previousPosition);
										
						if(
							match(Token(Token::KEYWORD, "case")) && match(Node::EXPRESSION) &&  
							match(Token(Token::OPERATOR, ":")) &&
							match(Node::OPERATOR) //#No SEMICOLON here!
						){
									result = true;
									break;
						}
						else recoil(previousPosition);


				} break;









				default: throw RecognitionException ("Some trash (" +currentToken.typeToText() + currentToken.getText() + ")on position " + currentToken.position.toString());
			}
		}
		log << "Result for " << Node(what).typeToText() << " on position "  << previousPosition << " is " << result<<"\n";
		log << "\tNow on " << currentPosition ;
		if(logDepth){
			log << " was on depth " << depth << "\n";
		}
		log << "\n";
		memo[make_pair(what, previousPosition)] = make_pair(result, currentPosition);

		depth --;
		//I don't think, this 'if' is necessary
		if(result == false){
			recoil(previousPosition);
		}
		return result;

	} 

	Node* get(const Token &what, bool ignore = false){
		
		//Rewrite, add 'recoil'
		if(ignore && match(what)){
			return new Node();
		}

		if(match(what)){
			return new Node(what);
		}
	}

	
	Node* get(Token::Type type, bool ignore = false){
		int previousPosition = currentPosition;
		Token what(type, ""); 

		if(ignore && match(what)){
			return new Node();
		}
		recoil(previousPosition);

		auto result = currentToken;
		if(match(what)){
			return new Node(result);
		}
		else {
			throw RecognitionException("Error in building tree on position " + to_string(currentPosition) + " (you should check what you are trying to get)");
		}

	}

/*	void push_right(Node *&to, Node *&what, Node::NodeType type){

	}
	*/
	void push_left(Node *what, Node *&to, Node::NodeType addType, Node::NodeType stopType){
		Node *current = to;
		while(current->children.size() > 0 && current->children[0]->type != stopType){
			current = current->children[0];
		}

		if(current->children.size() > 0){
			
		//	if(addType != current->type){
				what->children.push_back(current->children[0]);
				current->children[0] = new Node(addType);
				current->children[0]->children.push_back(what);
		//	}
		//	else {
		//		current->children.push_back(what);
		//	}
		}
		else {
			current->children.push_back(what);
		}
		
	}

	Node* get(Node::NodeType what, bool ignore = false){
				int previousPosition = currentPosition;
			//	log << previousPosition << "(prev pos)";
				Node *result = new Node(what);
				switch(what){
				case Node::VALUE: {				
									/*			if(match(Node::NAME) && match(Token::BRACKET_LEFT) && match(Node::EXPRESSION) && match(Token::BRACKET_RIGHT)){
													recoil(previousPosition);

													auto tmp = get(Node::NAME);
													auto tmp1 = new Node(Token(Token::OPERATOR, "[]"));
													tmp1->children.push_back(tmp);

													get(Token::BRACKET_LEFT, ignore);
													tmp1->children.push_back(get(Node::EXPRESSION));
													get(Token::BRACKET_RIGHT, ignore);

													result->children.push_back(tmp1);

													return result;

												}
												else recoil(previousPosition);

								/*				if(match(Node::FUNCALL) && match(Token::BRACKET_LEFT) && match(Node::EXPRESSION) && match(Token::BRACKET_RIGHT)){
													recoil(previousPosition);

													auto tmp = get(Node::FUNCALL);
													auto tmp1 = new Node(Token(Token::OPERATOR, "[]"));
													tmp1->children.push_back(tmp);

													get(Token::BRACKET_LEFT, ignore);
													tmp1->children.push_back(get(Node::EXPRESSION));
													get(Token::BRACKET_RIGHT, ignore);

													result->children.push_back(tmp1);

													return result;
												}
												else recoil(previousPosition);
							*/	
												if(match(Node::ARGLIST)){
													recoil(previousPosition);
													result->children.push_back(get(Node::ARGLIST));
													return result;
													
												}
												else recoil(previousPosition);

							/*					if(match(Node::FUNCALL) && match(Token::DOT) && match(Node::VALUE)){
													recoil(previousPosition);
													auto tmp = get(Node::FUNCALL);
													auto root = get(Token::DOT);
													root->children.push_back(tmp);
													root->children.push_back(get(Node::VALUE));
													result->children.push_back(root);
													return result;
												
												}
												else recoil(previousPosition);

								/*				if(match(Node::FUNCALL) && match(Token(Token::OPERATOR, "->")) && match(Node::VALUE)){
													recoil(previousPosition);
													auto tmp = get(Node::FUNCALL);

													auto root = get(Token(Token::OPERATOR, "->"));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::VALUE));
													result->children.push_back(root);

													return result;	
													
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL)){
													recoil(previousPosition);
													result->children.push_back(get(Node::FUNCALL));
													return result;
												}
												else recoil(previousPosition);
								*/
												if(match(Token::INT)){
													recoil(previousPosition);
													result->children.push_back(get(Token::INT));
													return result;
												}
												else recoil(previousPosition);

												if(match(Token::FLOAT)){
													recoil(previousPosition);
													result->children.push_back(get(Token::FLOAT));
													return result;
												}
												else recoil(previousPosition);

												if(match(Token::BOOL)){
													recoil(previousPosition);
													result->children.push_back(get(Token::BOOL));
													return result;
												}
												else recoil(previousPosition);

												if(match(Token::CHAR)){
													recoil(previousPosition);
													result->children.push_back(get(Token::CHAR));
													return result;
												}
												else recoil(previousPosition);

												if(match(Token::STRING)){
													recoil(previousPosition);
													result->children.push_back(get(Token::STRING));
													return result;
												}
												else recoil(previousPosition);

												
												if(match(Node::NAME)){
													recoil(previousPosition);

													result->children.push_back(get(Node::NAME));
													return result;
												}
												else recoil(previousPosition);


												
											} break;

				case Node::BRACED: 			{
												if(match(Node::ARGLIST) && match(Node::BRACED)){
													recoil(previousPosition);
													auto tmp = get(Node::ARGLIST);
												
													auto tmp2 = get(Node::BRACED);
													push_left(tmp, tmp2, Node::SPECIAL, Node::END);
													return tmp2;
												}
												else recoil(previousPosition);

												if(match(Node::ACCESS) && match(Node::BRACED)){
													recoil(previousPosition);
													auto tmp = get(Node::ACCESS);
												
													auto tmp2 = get(Node::BRACED);

													push_left(tmp, tmp2, Node::ACCESS, Node::END);
													return tmp2;
												}
												else recoil(previousPosition);

												if(match(Node::ARGLIST)){
													recoil(previousPosition);

													result->children.push_back(get(Node::ARGLIST));

													return result;	
												}
												else recoil(previousPosition);

												if(match(Node::ACCESS)){
													recoil(previousPosition);

													result->children.push_back(get(Node::ACCESS));

													return result;	
												}
												else recoil(previousPosition);
											
											} break;



				case Node::ACCESS: 			{										
												if(match(Token::BRACKET_LEFT) && match(Node::EXPRESSION) && match(Token::BRACKET_RIGHT)){
													recoil(previousPosition);
													get(Token::BRACKET_LEFT, ignore);
													result->children.push_back(get(Node::EXPRESSION));
													get(Token::BRACKET_RIGHT, ignore);
													return result;
												}
												else recoil(previousPosition);

											} break;

				case Node::ACCESS_EXPR:      {
												if(match(Node::EXPR8) && match(Node::ACCESS)){
													recoil(previousPosition);
													result->children.push_back(get(Node::EXPR8));
													result->children.push_back(get(Node::ACCESS));
													return result;
												}
												else recoil(previousPosition);

											}

				case Node::EXPR8: 			{
												if(match(Token::BRACE_LEFT) && match(Node::EXPRESSION) && match(Token::BRACE_RIGHT)){
													recoil(previousPosition);
													get(Token::BRACE_LEFT, ignore);
													result->children.push_back(get(Node::EXPRESSION));
													get(Token::BRACE_RIGHT, ignore);
													return result;
												}
												else recoil(previousPosition);

												if (match(Node::VALUE)) {
													recoil(previousPosition);
													result->children.push_back(get(Node::VALUE));
													return result;
												//	return get(Node::VALUE);
												}
												else recoil(previousPosition);
											
											} break;

				case Node::EXPR7: 			{										
												if(match(Node::FUNCALL) && match(Node::BRACED)){
													
													recoil(previousPosition);
													auto tmp = get(Node::FUNCALL);
												
													auto tmp2 = get(Node::BRACED);

													push_left(tmp, tmp2, Node::FUNCALL, Node::END);
													return tmp2;
												}
												else recoil(previousPosition);


												if(match(Node::ACCESS_EXPR) && match(Node::BRACED)){
													
													recoil(previousPosition);
													auto tmp = get(Node::EXPR8);
													
													auto tmp2 = get(Node::BRACED);

													push_left(tmp, tmp2, Node::FUNCALL, Node::END);
													return tmp2;
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL)){
													
													recoil(previousPosition);
													result->children.push_back(get(Node::FUNCALL));
													return result;
												}
												else recoil(previousPosition);


												if(match(Node::ACCESS_EXPR)){
													
													recoil(previousPosition);
													result->children.push_back(get(Node::ACCESS_EXPR));
													return result;
												}
												else recoil(previousPosition);

	
												if(match(Node::EXPR8)){
													recoil(previousPosition);
													result->children.push_back(get(Node::EXPR8));
													return result;
												//	return get(Node::EXPR8);
												}
												else recoil(previousPosition);
											} break;	
			case Node::EXPR6: 			{		
												//cout << (match(Node::EXPR7));//
												// && match(Token(Token::OPERATOR, "->" )));// && match(Node::EXPR6));
												//cout << currentToken.text;
												//recoil(previousPosition);
												if(match(Node::EXPR7) && match(Token(Token::OPERATOR, "->" )) && match(Node::EXPR6) ){
													recoil(previousPosition);
												//	cout << "asdfasdfasfdasd";
													auto tmp = get(Node::EXPR7);

													auto left = get(Token(Token::OPERATOR, "->"));
													left->children.push_back(tmp);

												//	result->children.push_back(left);
																										
												//	result->children.push_back(get(Node::EXPR2));
													auto tmp2 = get(Node::EXPR6);

													push_left(left, tmp2, Node::EXPR6, Node::EXPR7);

												//	result->children.push_back(root);

												//	cout << "asdfasdfasdfasdf";
													return tmp2;
												/*	recoil(previousPosition);
													auto tmp = get(Node::EXPR7);

													auto root = get(Token(Token::OPERATOR, "->"));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPR6));
													result->children.push_back(root);

													return result;	
												*/
												}
												else recoil(previousPosition);

												if(match(Node::EXPR7) && match(Token::DOT) && match(Node::EXPR6)){
													recoil(previousPosition);
													auto tmp = get(Node::EXPR7);

													auto left = get(Token(Token::DOT));
													left->children.push_back(tmp);

												//	result->children.push_back(left);
																										
												//	result->children.push_back(get(Node::EXPR2));
													auto tmp2 = get(Node::EXPR6);

													push_left(left, tmp2, Node::EXPR6, Node::EXPR7);

												//	result->children.push_back(root);


													return tmp2;
												/*	recoil(previousPosition);
													auto tmp = get(Node::EXPR7);

													auto root = get(Token::DOT);
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPR6));
													result->children.push_back(root);

													return result;	
													*/
												}
												else recoil(previousPosition);

												if(match(Token(Token::OPERATOR, "*" )) && match(Node::EXPR7)){
													recoil(previousPosition);
													auto root = get(Token(Token::OPERATOR, "*" ));
					
													root->children.push_back(get(Node::EXPR7));
													result->children.push_back(root);

													return result;	
												}
												else recoil(previousPosition);

												if(match(Node::EXPR7)){
													recoil(previousPosition);
													result->children.push_back(get(Node::EXPR7));
												//	return get(Node::EXPR7);
													return result;
												}
												else recoil(previousPosition);
											
											} break;

			case Node::EXPR5: 			{										
												if(match(Node::EXPR6) && match(Token(Token::OPERATOR, "**")) && match(Node::EXPR5)){
													recoil(previousPosition);
													auto tmp = get(Node::EXPR6);

													auto root = get(Token(Token::OPERATOR, "**"));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPR5));
													result->children.push_back(root);

													return result;	
												}
												else recoil(previousPosition);

												if(match(Node::EXPR6)){
													recoil(previousPosition);
													result->children.push_back(get(Node::EXPR6));
													return result;
												//	return get(Node::EXPR6);
												}
												else recoil(previousPosition);
											} break;	

				case Node::EXPR4: 			{
												if(match(Token(Token::OPERATOR, "~")) && match(Node::EXPR5)){

													recoil(previousPosition);

													auto root = get(Token(Token::OPERATOR, "~"));
																										
													root->children.push_back(get(Node::EXPR5));
													result->children.push_back(root);

													return result;	
												
												}
												else recoil(previousPosition);
												if (match(Token(Token::OPERATOR, "!")) && match(Node::EXPR5)) {
													recoil(previousPosition);

													auto root = get(Token(Token::OPERATOR, "!"));
																										
													root->children.push_back(get(Node::EXPR5));
													result->children.push_back(root);

													return result;	
												}
												else recoil(previousPosition);
												if(match(Token(Token::OPERATOR, "-")) && match(Node::EXPR5)){
													recoil(previousPosition);

													auto root = get(Token(Token::OPERATOR, "-"));
																										
													root->children.push_back(get(Node::EXPR5));
													result->children.push_back(root);

													return result;	
												}
												else recoil(previousPosition);
												if(match(Token(Token::OPERATOR, "+")) && match(Node::EXPR5)){
													recoil(previousPosition);

													auto root = get(Token(Token::OPERATOR, "+"));
																										
													root->children.push_back(get(Node::EXPR5));
													result->children.push_back(root);

													return result;	
												}
												else recoil(previousPosition);

												if(match(Node::EXPR5)){
													recoil(previousPosition);

													result->children.push_back(get(Node::EXPR5));
													return result;	
													//return get(Node::EXPR5);
												}
												else recoil(previousPosition);


											} break;

				

				case Node::EXPR3: 			{										
												if(match(Node::EXPR4) && match(Token(Token::OPERATOR, "*")) && match(Node::EXPR3)){
													recoil(previousPosition);
													auto tmp = get(Node::EXPR4);

													auto left = get(Token(Token::OPERATOR, "*"));
													left->children.push_back(tmp);

												//	result->children.push_back(left);
																										
												//	result->children.push_back(get(Node::EXPR2));
													auto tmp2 = get(Node::EXPR3);

													push_left(left, tmp2, Node::EXPR3, Node::EXPR4);

												//	result->children.push_back(root);


													return tmp2;
												/*	recoil(previousPosition);
													auto tmp = get(Node::EXPR4);

													auto root = get(Token(Token::OPERATOR, "*"));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPR3));
													result->children.push_back(root);

													return result;
												*/	
												}
												else recoil(previousPosition);

												if(match(Node::EXPR4) && match(Token(Token::OPERATOR, "/")) && match(Node::EXPR3)){
													recoil(previousPosition);
													auto tmp = get(Node::EXPR4);

													auto left = get(Token(Token::OPERATOR, "/"));
													left->children.push_back(tmp);

												//	result->children.push_back(left);
																										
												//	result->children.push_back(get(Node::EXPR2));
													auto tmp2 = get(Node::EXPR3);

													push_left(left, tmp2, Node::EXPR3, Node::EXPR4);

												//	result->children.push_back(root);


													return tmp2;
												/*	recoil(previousPosition);
													auto tmp = get(Node::EXPR4);

													auto root = get(Token(Token::OPERATOR, "/"));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPR3));
													result->children.push_back(root);

													return result;	
												*/
												}
												else recoil(previousPosition);

												if(match(Node::EXPR4) && match(Token(Token::OPERATOR, "%")) && match(Node::EXPR3)){
													recoil(previousPosition);
													auto tmp = get(Node::EXPR4);

													auto left = get(Token(Token::OPERATOR, "%"));
													left->children.push_back(tmp);

												//	result->children.push_back(left);
																										
												//	result->children.push_back(get(Node::EXPR2));
													auto tmp2 = get(Node::EXPR3);

													push_left(left, tmp2, Node::EXPR3, Node::EXPR4);

												//	result->children.push_back(root);


													return tmp2;
												/*	recoil(previousPosition);
													auto tmp = get(Node::EXPR4);

													auto root = get(Token(Token::OPERATOR, "%"));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPR3));
													result->children.push_back(root);

													return result;
													*/
												}
												else recoil(previousPosition);

												if(match(Node::EXPR4)){
													recoil(previousPosition);
													result->children.push_back(get(Node::EXPR4));
													return result;
												//	return get(Node::EXPR4);
												}
												else recoil(previousPosition);
											} break;	

					case Node::EXPR2: 			{										
												if(match(Node::EXPR3) && match(Token(Token::OPERATOR, "+")) && match(Node::EXPR2)){
													recoil(previousPosition);
													auto tmp = get(Node::EXPR3);

													auto left = get(Token(Token::OPERATOR, "+"));
													left->children.push_back(tmp);

												//	result->children.push_back(left);
																										
												//	result->children.push_back(get(Node::EXPR2));
													auto tmp2 = get(Node::EXPR2);

													push_left(left, tmp2, Node::EXPR2, Node::EXPR3);

												//	result->children.push_back(root);


													return tmp2;
														
												/*	recoil(previousPosition);
													auto tmp = get(Node::EXPR3);

													auto root = get(Token(Token::OPERATOR, "+"));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPR2));
													result->children.push_back(root);

													return result;
												*/	
												}
												else recoil(previousPosition);

												if(match(Node::EXPR3) && match(Token(Token::OPERATOR, "-")) && match(Node::EXPR2)){

													recoil(previousPosition);
													auto tmp = get(Node::EXPR3);

													auto left = get(Token(Token::OPERATOR, "-"));
													left->children.push_back(tmp);

												//	result->children.push_back(left);
																										
												//	result->children.push_back(get(Node::EXPR2));
													auto tmp2 = get(Node::EXPR2);

													push_left(left, tmp2, Node::EXPR2, Node::EXPR3);

												//	result->children.push_back(root);


													return tmp2;
													/*
													recoil(previousPosition);
													auto tmp = get(Node::EXPR3);

													auto root = get(Token(Token::OPERATOR, "-"));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPR2));
													result->children.push_back(root);

													return result;
													*/
												}
												else recoil(previousPosition);

												if(match(Node::EXPR3)){
													recoil(previousPosition);
													result->children.push_back(get(Node::EXPR3));
													return result;
												//$	return get(Node::EXPR3);
												}
												else recoil(previousPosition);
											} break;	

					case Node::EXPR1: 			{										
												if(match(Node::EXPR2) && match(Token(Token::OPERATOR, "<<")) && match(Node::EXPR1)){
												/*	recoil(previousPosition);
													auto tmp = get(Node::EXPR2);

													auto root = get(Token(Token::OPERATOR, "<<"));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPR1));
													result->children.push_back(root);

													return result;
													*/

													recoil(previousPosition);
													auto tmp = get(Node::EXPR2);
													auto left = get(Token(Token::OPERATOR, "<<"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::EXPR1);
													push_left(left, tmp2, Node::EXPR1, Node::EXPR2);
													return tmp2;
												}
												else recoil(previousPosition);

												if(match(Node::EXPR2) && match(Token(Token::OPERATOR, ">>")) && match(Node::EXPR1)){
													recoil(previousPosition);
													auto tmp = get(Node::EXPR2);
													auto left = get(Token(Token::OPERATOR, ">>"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::EXPR1);
													push_left(left, tmp2, Node::EXPR1, Node::EXPR2);
													return tmp2;
												}
												else recoil(previousPosition);


												if(match(Node::EXPR2)){
													recoil(previousPosition);
													result->children.push_back(get(Node::EXPR2));
													return result;
												//	return get(Node::EXPR2);
												}
												else recoil(previousPosition);
											} break;	

					case Node::B_AND: 			{										
												if(match(Node::EXPR1) && match(Token(Token::OPERATOR, "&")) && match(Node::B_AND)){
													recoil(previousPosition);
													auto tmp = get(Node::EXPR1);
													auto left = get(Token(Token::OPERATOR, "&"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::B_AND);
													push_left(left, tmp2, Node::B_AND, Node::EXPR1);
													return tmp2;
												}
												else recoil(previousPosition);


												if(match(Node::EXPR1)){
													recoil(previousPosition);
													result->children.push_back(get(Node::EXPR1));
													return result;
													//return get(Node::EXPR1);
												}
												else recoil(previousPosition);
											} break;

					case Node::B_XOR: 			{										
												if(match(Node::B_AND) && match(Token(Token::OPERATOR, "^")) && match(Node::B_XOR)){
													recoil(previousPosition);
													auto tmp = get(Node::B_AND);
													auto left = get(Token(Token::OPERATOR, "<<"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::B_XOR);
													push_left(left, tmp2, Node::B_XOR, Node::B_AND);
													return tmp2;
												}
												else recoil(previousPosition);


												if(match(Node::B_AND)){
													recoil(previousPosition);
													result->children.push_back(get(Node::B_AND));
													return result;
												//	return get(Node::B_AND);
												}
												else recoil(previousPosition);
											} break;

					case Node::B_OR: 			{										
												if(match(Node::B_XOR) && match(Token(Token::OPERATOR, "|")) && match(Node::B_OR)){
													recoil(previousPosition);
													auto tmp = get(Node::B_XOR);
													auto left = get(Token(Token::OPERATOR, "|"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::B_OR);
													push_left(left, tmp2, Node::B_OR, Node::B_XOR);
													return tmp2;
												}
												else recoil(previousPosition);


												if(match(Node::B_XOR)){
													recoil(previousPosition);
													result->children.push_back(get(Node::B_XOR));
													return result;
													//return get(Node::B_XOR);
												}
												else recoil(previousPosition);
											} break;

				case Node::COMPARISION: {										
												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "==")) && match(Node::COMPARISION)){
													recoil(previousPosition);
													auto tmp = get(Node::B_OR);
													auto left = get(Token(Token::OPERATOR, "=="));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::COMPARISION);
													push_left(left, tmp2, Node::COMPARISION, Node::B_OR);
													return tmp2;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, ">")) && match(Node::COMPARISION)){
													recoil(previousPosition);
													auto tmp = get(Node::B_OR);
													auto left = get(Token(Token::OPERATOR, ">"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::COMPARISION);
													push_left(left, tmp2, Node::COMPARISION, Node::B_OR);
													return tmp2;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "<")) && match(Node::COMPARISION)){
													recoil(previousPosition);
													auto tmp = get(Node::B_OR);
													auto left = get(Token(Token::OPERATOR, "<"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::COMPARISION);
													push_left(left, tmp2, Node::COMPARISION, Node::B_OR);
													return tmp2;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, ">=")) && match(Node::COMPARISION)){
													recoil(previousPosition);
													auto tmp = get(Node::B_OR);
													auto left = get(Token(Token::OPERATOR, ">="));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::COMPARISION);
													push_left(left, tmp2, Node::COMPARISION, Node::B_OR);
													return tmp2;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "<=")) && match(Node::COMPARISION)){
													recoil(previousPosition);
													auto tmp = get(Node::B_OR);
													auto left = get(Token(Token::OPERATOR, "<="));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::COMPARISION);
													push_left(left, tmp2, Node::COMPARISION, Node::B_OR);
													return tmp2;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "!=")) && match(Node::COMPARISION)){
													recoil(previousPosition);
													auto tmp = get(Node::B_OR);
													auto left = get(Token(Token::OPERATOR, "!="));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::COMPARISION);
													push_left(left, tmp2, Node::COMPARISION, Node::B_OR);
													return tmp2;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR)){
													recoil(previousPosition);
													result->children.push_back(get(Node::B_OR));
													return result;
													//return get(Node::B_OR);
												}
												else recoil(previousPosition);

											} break;


						case Node::IS_IN_EXPRESSION: {
										if(match(Node::COMPARISION) && match(Token(Token::KEYWORD, "is")) && match(Node::IS_IN_EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::COMPARISION);
													auto left = get(Token(Token::KEYWORD, "is"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::IS_IN_EXPRESSION);
													push_left(left, tmp2, Node::IS_IN_EXPRESSION, Node::COMPARISION);
													return tmp2;
										}
										else recoil(previousPosition);

										if(match(Node::COMPARISION) && match(Token(Token::KEYWORD, "in")) && match(Node::IS_IN_EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::COMPARISION);
													auto left = get(Token(Token::KEYWORD, "in"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::IS_IN_EXPRESSION);
													push_left(left, tmp2, Node::IS_IN_EXPRESSION, Node::COMPARISION);
													return tmp2;
										}
										else recoil(previousPosition);

										if(match(Node::COMPARISION)){
													recoil(previousPosition);
													result->children.push_back(get(Node::COMPARISION));
													return result;
													//return get(Node::COMPARISION);
										}
										else recoil(previousPosition);


									} break;

					case Node::L_AND: 			{										
												if(match(Node::IS_IN_EXPRESSION) && match(Token(Token::OPERATOR, "&&")) && match(Node::L_AND)){
													recoil(previousPosition);
													auto tmp = get(Node::IS_IN_EXPRESSION);
													auto left = get(Token(Token::OPERATOR, "&&"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::L_AND);
													push_left(left, tmp2, Node::L_AND, Node::IS_IN_EXPRESSION);
													return tmp2;
												}
												else recoil(previousPosition);


												if(match(Node::IS_IN_EXPRESSION)){
													recoil(previousPosition);
													result->children.push_back(get(Node::IS_IN_EXPRESSION));
													return result;
													//return get(Node::IS_IN_EXPRESSION);
												}
												else recoil(previousPosition);
											} break;

					case Node::L_OR: 			{										
												if(match(Node::L_AND) && match(Token(Token::OPERATOR, "||")) && match(Node::L_OR)){
													recoil(previousPosition);
													auto tmp = get(Node::L_AND);
													auto left = get(Token(Token::OPERATOR, "||"));
													left->children.push_back(tmp);
													auto tmp2 = get(Node::L_OR);
													push_left(left, tmp2, Node::L_OR, Node::L_AND);
													return tmp2;
												}
												else recoil(previousPosition);


												if(match(Node::L_AND)){
													recoil(previousPosition);
													result->children.push_back(get(Node::L_AND));
													return result;
													//return get(Node::L_AND);
												}
												else recoil(previousPosition);
											} break;


					case Node::EXPRESSION: {			
												if(match(Node::ASSIGNMENT)){
													recoil(previousPosition);
													result->children.push_back(get(Node::ASSIGNMENT));
													return result;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR)){
													recoil(previousPosition);
													result->children.push_back(get(Node::L_OR));
													return result;
												}
												else recoil(previousPosition);
								

												if(match(Node::VALUE)){
													recoil(previousPosition);
													result->children.push_back(get(Node::VALUE));
													return result;
												}	


											} break;

					case Node::ASSIGNMENT: {		

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "=")) && match(Node::EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::L_OR);

													auto root = get(Token(Token::OPERATOR, "="));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPRESSION));
													result->children.push_back(root);

													return result;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "+=")) && match(Node::EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::L_OR);

													auto root = get(Token(Token::OPERATOR, "+="));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPRESSION));
													result->children.push_back(root);

													return result;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "-=")) && match(Node::EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::L_OR);

													auto root = get(Token(Token::OPERATOR, "-="));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPRESSION));
													result->children.push_back(root);

													return result;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "/=")) && match(Node::EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::L_OR);

													auto root = get(Token(Token::OPERATOR, "/="));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPRESSION));
													result->children.push_back(root);

													return result;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "%=")) && match(Node::EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::L_OR);

													auto root = get(Token(Token::OPERATOR, "%="));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPRESSION));
													result->children.push_back(root);

													return result;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "*=")) && match(Node::EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::L_OR);

													auto root = get(Token(Token::OPERATOR, "*="));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPRESSION));
													result->children.push_back(root);

													return result;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "|=")) && match(Node::EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::L_OR);

													auto root = get(Token(Token::OPERATOR, "|="));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPRESSION));
													result->children.push_back(root);

													return result;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "||=")) && match(Node::EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::L_OR);

													auto root = get(Token(Token::OPERATOR, "||="));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPRESSION));
													result->children.push_back(root);

													return result;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "&=")) && match(Node::EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::L_OR);

													auto root = get(Token(Token::OPERATOR, "&="));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPRESSION));
													result->children.push_back(root);

													return result;
												}
												else recoil(previousPosition);

												if(match(Node::L_OR) && match(Token(Token::OPERATOR, "&&=")) && match(Node::EXPRESSION)){
													recoil(previousPosition);
													auto tmp = get(Node::L_OR);

													auto root = get(Token(Token::OPERATOR, "&&="));
													root->children.push_back(tmp);
																										
													root->children.push_back(get(Node::EXPRESSION));
													result->children.push_back(root);

													return result;
												}
												else recoil(previousPosition);



											} break;




					case Node::FUNCALL: {
										if(match(Node::EXPR8) && match(Node::ARGLIST)){
											recoil(previousPosition);
											result->children.push_back(get(Node::EXPR8));
											result->children.push_back(get(Node::ARGLIST));
											return result;
										}
										else recoil(previousPosition);



										} break;


					case Node::NAME: {
										if(match(Node::NAMESPACE) && match(Token(Token::OPERATOR, "::")) && match(Node::NAME)){
													recoil(previousPosition);
													auto tmp = get(Node::NAMESPACE);
													auto tmp2 = get(Token(Token::OPERATOR, "::"));
													tmp2->children.push_back(tmp);
													tmp2->children.push_back(get(Node::NAME));
													result->children.push_back(tmp2);
													return result;
											}
										else recoil(previousPosition);
										
									
						/*				if(match(Token(Token::OPERATOR, "*")) && match(Node::NAME1)){
													recoil(previousPosition);
													auto tmp = get(Token(Token::OPERATOR, "*"));
													tmp->children.push_back(get(Node::NAME1));
													result->children.push_back(tmp);
													return result;
											}
										else recoil(previousPosition);

										if(match(Node::NAME1) && match(Token(Token::OPERATOR, "->")) && match(Node::NAME)){
													recoil(previousPosition);
													auto tmp = get(Node::NAME1);
													auto tmp2 = get(Token(Token::OPERATOR, "->"));
													tmp2->children.push_back(tmp);
													tmp2->children.push_back(get(Node::NAME));
													result->children.push_back(tmp2);
													return result;
											}
										else recoil(previousPosition);
										
										if(match(Node::NAME1) && match(Token(Token::DOT, ".")) && match(Node::NAME)){
													recoil(previousPosition);
													auto tmp = get(Node::NAME1);
													auto tmp2 = get(Token(Token::DOT, "."));
													tmp2->children.push_back(tmp);
													tmp2->children.push_back(get(Node::NAME));
													result->children.push_back(tmp2);
													return result;
											}
											else recoil(previousPosition); 
										
							*/
										if(match(Node::NAME1)){
												recoil(previousPosition);
												result->children.push_back(get(Node::NAME1));
												return result;
										}
										else recoil(previousPosition);



										} break;

					case Node::NAME1: {	

	
											if(match(Token::NAME)){
								
												recoil(previousPosition);											
											//#	result->children.push_back(get(Token::NAME));
											//#	return result;
												return get(Token::NAME);
											}
											else recoil(previousPosition);

																						
											if(match(Token::BRACE_LEFT) && match(Node::NAME) && match(Token::BRACE_RIGHT)){
													recoil(previousPosition);
													get(Token::BRACE_LEFT, ignore);
													result->children.push_back(get(Node::NAME));
													get(Token::BRACE_RIGHT, ignore);
													return result;
											}
											else recoil(previousPosition);

											

										} break;
					case Node::NAMESPACE: {
											if(match(Token::NAME)){
													recoil(previousPosition);
													result->children.push_back(get(Token::NAME));
													return result;
											}
											else recoil(previousPosition);

										} break;

					case Node::LIST: {	
										

										if(match(Node::LIST_ELEM) && match(Token::COMMA) && match(Node::LIST)){
													recoil(previousPosition);
													auto tmp = get(Node::LIST_ELEM);
													get(Token::COMMA, ignore);
													result->children = get(Node::LIST)->children; //#!!!!It is not a mistake!
													result->children.insert(result->children.begin(), tmp);
													return result;
													
										}
										else recoil(previousPosition);

										if(match(Node::LIST_ELEM)){
													recoil(previousPosition);
													result->children.push_back(get(Node::LIST_ELEM));
													return result;
												
										}
										else recoil(previousPosition);
									} break;		
					case Node::LIST_ELEM: {	
										

										if(match(Token::BRACE_LEFT) && match(Node::LIST) && match(Token::BRACE_RIGHT)){
													recoil(previousPosition);
													get(Token::BRACE_LEFT, ignore);
													result->children.push_back(get(Node::LIST));
													get(Token::BRACE_RIGHT, ignore);
													return result;			
										}
										else recoil(previousPosition);

									if(match(Node::ASSIGNMENT)){
													recoil(previousPosition);
													result->children.push_back(get(Node::ASSIGNMENT));
													return result;
										}
										else recoil(previousPosition);

										if(match(Node::L_OR)){
													recoil(previousPosition);
												result->children.push_back(get(Node::L_OR));
												return result;
										}
										else recoil(previousPosition);
				

										

										if(match(Node::VALUE)){
												recoil(previousPosition);
												result->children.push_back(get(Node::VALUE));
												return result;
										}
										else recoil(previousPosition);


									} break;	
				
					case Node::ARGLIST: {
									if(match(Token::BRACE_LEFT) && match(Node::LIST) && match(Token::BRACE_RIGHT)){
													recoil(previousPosition);
													get(Token::BRACE_LEFT, ignore);
													result->children.push_back(get(Node::LIST));
													get(Token::BRACE_RIGHT, ignore);
													return result;
																							
									}
									else recoil(previousPosition);

									if(match(Token::BRACE_LEFT) && match(Token::BRACE_RIGHT)){
													recoil(previousPosition);
													get(Token::BRACE_LEFT, ignore);
													get(Token::BRACE_RIGHT, ignore);

													return result;
									}
									else recoil(previousPosition);

									} break;

				
									
					case Node::OPERATOR: { //# OPTIMIZE NONE OF THESE!
										if(match(Node::SPECIAL)){
													recoil(previousPosition);
													result->children.push_back(get(Node::SPECIAL));
													return result;
										}
										else recoil(previousPosition);
									
										if(match(Node::FUNCDEF)){
											recoil(previousPosition);

											result->children.push_back(get(Node::FUNCDEF));
											return result;
										}
										else recoil(previousPosition);

										if(match(Node::DECLARATION) && match(Token::SEMICOLON)){
													recoil (previousPosition);
													result->children.push_back(get(Node::DECLARATION));
													get(Token::SEMICOLON, ignore);
													return result;
										}
										else recoil(previousPosition);

										if(match(Node::EXPRESSION) && match(Token::SEMICOLON)){
													recoil(previousPosition);
													result->children.push_back(get(Node::EXPRESSION));
													get(Token::SEMICOLON, ignore);
													return result;
											
										}
										else recoil(previousPosition);

										if(match(Node::BODY)){
												recoil(previousPosition);
												result->children.push_back(get(Node::BODY));
												return result;												
										}
										else recoil(previousPosition);

										if(match(Token::SEMICOLON)){
													return result;
										}
										else recoil(previousPosition);



									} break;

				case Node::OPERATORS: {
										if(match(Node::OPERATOR) &&  match(Node::OPERATORS)){
													recoil(previousPosition);
													auto tmp = get(Node::OPERATOR);
													result->children = get(Node::OPERATORS)->children; //# !!!!It is not a mistake!
													result->children.insert(result->children.begin(), tmp);
													return result;											
										}
										else recoil(previousPosition);

										if(true){
											return result;												
										}
										else recoil(previousPosition);



									} break;				

				case Node::BODY: {
										if(match(Token::CURL_LEFT) &&  match(Node::OPERATORS) && match(Token::CURL_RIGHT)){
													recoil(previousPosition);
													get(Token::CURL_LEFT, ignore);
													result->children.push_back(get(Node::OPERATORS));
													get(Token::CURL_RIGHT, ignore);
													return result;													
										}
										else recoil(previousPosition);


									} break;

				case Node::DECLARATION: {
										if(match(Node::TYPE) &&  match(Node::VARLIST)) {
											recoil(previousPosition);
													result->children.push_back(get(Node::TYPE));
													result->children.push_back(get(Node::VARLIST));
													return result;
										}
										else recoil(previousPosition);


									} break;

				case Node::VARLIST: {	


										if(match(Node::ASSIGNMENT) && match(Token::COMMA) && match(Node::VARLIST)) {
													recoil(previousPosition);
													auto tmp = get(Node::ASSIGNMENT);
													get(Token::COMMA, ignore);
													result->children = get(Node::VARLIST)->children; //# !!!!It is not a mistake!
													result->children.insert(result->children.begin(), tmp);
													return result;
										}
										else recoil(previousPosition);



										if(match(Node::ASSIGNMENT)) {
												recoil(previousPosition);
												result->children.push_back(get(Node::ASSIGNMENT));
												return result;
										}
										else recoil(previousPosition);

										if(match(Node::NAME) && match(Token::COMMA) && match(Node::VARLIST)) {
													recoil(previousPosition);
													auto tmp = get(Node::NAME);
													get(Token::COMMA, ignore);
													result->children = get(Node::VARLIST)->children; //# !!!!It is not a mistake!
													result->children.insert(result->children.begin(), tmp);
													return result;
										}
										else recoil(previousPosition);



										if(match(Node::NAME)) {
												recoil(previousPosition);
												result->children.push_back(get(Node::NAME));
												return result;

										}
										else recoil(previousPosition);

									} break;			


				case Node::TYPE: { //# DO NOT OPTIMIZE THIS!
										if(match(Node::NAME)) {
											recoil(previousPosition);
												result->children.push_back(get(Node::NAME));
												return result;
										}
										else recoil(previousPosition);


									} break;

				case Node::FUNCDEF: {
										if(match(Node::TYPE) && match(Node::NAME) && match(Token::BRACE_LEFT) && match(Node::DECL_LIST) && match(Token::BRACE_RIGHT) && match(Node::BODY)) {
												recoil(previousPosition);
												result->children.push_back(get(Node::TYPE));
												result->children.push_back(get(Node::NAME));
												get(Token::BRACE_LEFT, ignore);
												result->children.push_back(get(Node::DECL_LIST));
												get(Token ::BRACE_RIGHT, ignore);
												result->children.push_back(get(Node::BODY));
												return result;
										}
										else recoil(previousPosition);


									} break;
				case Node::DECL_LIST: {
										if(match(Node::DECL_ATOM) && match(Token::COMMA) && match(Node::DECL_LIST)) {
													recoil(previousPosition);
													auto tmp = get(Node::DECL_ATOM);
													get(Token::COMMA, ignore);
													result->children = get(Node::DECL_LIST)->children; //# !!!!It is not a mistake!
													result->children.insert(result->children.begin(), tmp);
													return result;
										}
										else recoil(previousPosition);

										if(match(Node::DECL_ATOM)) {
											recoil(previousPosition);
													result->children.push_back(get(Node::DECL_ATOM));
													return result;
										}
										else recoil(previousPosition);

										if(true) {
												return result;
										}
										else recoil(previousPosition);


									} break;
				case Node::DECL_ATOM: {
										if(match(Node::TYPE) && match(Node::ASSIGNMENT)) {
											recoil(previousPosition);
												result->children.push_back(get(Node::TYPE));
												result->children.push_back(get(Node::ASSIGNMENT));
												return result;
										}
										else recoil(previousPosition);

										if(match(Node::TYPE) && match(Node::NAME)) {
													recoil(previousPosition);
												result->children.push_back(get(Node::TYPE));
												result->children.push_back(get(Node::NAME));
												return result;
										}
										else recoil(previousPosition);


									} break;

				case Node::FORTHING: {	
										if(match(Node::DECLARATION) ){
												recoil(previousPosition);
												result->children.push_back(get(Node::DECLARATION));
												return result;

										}
										else recoil(previousPosition);

										if(match(Node::EXPRESSION)){
												recoil(previousPosition);
												result->children.push_back(get(Node::EXPRESSION));
												return result;
										}
										else recoil(previousPosition);

										
										

										if(match(Node::BODY)){
												recoil(previousPosition);
												result->children.push_back(get(Node::BODY));
												return result;
										}
										else recoil(previousPosition);

										if(true){
											return result;
										}
										else recoil(previousPosition);

									} break;
				case Node::SPECIAL: {

								if(match(Token(Token::KEYWORD, "foreach")) && match(Token::BRACE_LEFT) &&
										match(Node::DECLARATION) && match(Token(Token::OPERATOR, ":")) && match(Node::EXPRESSION)&&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) 
								 ) {
								 				recoil(previousPosition);
												auto tmp = get(Token(Token::KEYWORD, "foreach"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);
												auto tmp2 = get(Node::DECLARATION);
												tmp->children.push_back(get(Token(Token::OPERATOR, ":")));
												tmp->children[0]->children.push_back(tmp2);
												tmp->children[0]->children.push_back(get(Node::EXPRESSION));
												get(Token::BRACE_RIGHT, ignore);
												tmp->children.push_back(get(Node::OPERATOR));
												result->children.push_back(tmp);
											//#	return result;
												return tmp;

										}
									else recoil(previousPosition);

								if(match(Token(Token::KEYWORD, "for")) && match(Token::BRACE_LEFT) &&
										match(Node::FORTHING) && match(Token::SEMICOLON) && 
										match(Node::FORTHING) && match(Token::SEMICOLON) && 
										match(Node::FORTHING) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) 
								 ) {
												recoil(previousPosition);
												auto tmp = get(Token(Token::KEYWORD, "for"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::FORTHING));
												get(Token::SEMICOLON, ignore);

												tmp->children.push_back(get(Node::FORTHING));
												get(Token::SEMICOLON, ignore);

												tmp->children.push_back(get(Node::FORTHING));

												get(Token::BRACE_RIGHT, ignore);
												tmp->children.push_back(get(Node::OPERATOR));
												result->children.push_back(tmp);
											//#	return result;
												return tmp;


										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "if")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
													recoil(previousPosition);
												auto tmp = get(Token(Token::KEYWORD, "if"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::EXPRESSION));

												get(Token::BRACE_RIGHT, ignore);

												tmp->children.push_back(get(Node::OPERATOR));

												get(Token(Token::KEYWORD, "else"), ignore);
												tmp->children.push_back(get(Node::OPERATOR));
												result->children.push_back(tmp);
											//#	return result;
												return tmp;
										}
									else recoil(previousPosition);

								if(match(Token(Token::KEYWORD, "if")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) 
								 ) {
													recoil(previousPosition);
												auto tmp = get(Token(Token::KEYWORD, "if"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::EXPRESSION));

												get(Token::BRACE_RIGHT, ignore);

												tmp->children.push_back(get(Node::OPERATOR));


												result->children.push_back(tmp);
										//#		return result;
												return tmp;
										}

									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "finally")) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
												recoil(previousPosition);
												auto tmp = get(Token(Token::KEYWORD, "while"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::EXPRESSION));

												get(Token::BRACE_RIGHT, ignore);

												auto init_part = new Node();

												auto do_part = get(Node::OPERATOR);

												get(Token(Token::KEYWORD, "finally"), ignore);

												auto finally_part = get(Node::OPERATOR);

												get(Token(Token::KEYWORD, "else"), ignore);

												auto else_part = get(Node::OPERATOR);

												tmp->children.push_back(else_part);
												tmp->children.push_back(init_part);
												tmp->children.push_back(do_part);
												tmp->children.push_back(finally_part);
												result->children.push_back(tmp);
											//#	return result;
												return tmp;
										}
									else recoil(previousPosition);


								if(		match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
												recoil(previousPosition);
												auto tmp = get(Token(Token::KEYWORD, "while"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::EXPRESSION));

												get(Token::BRACE_RIGHT, ignore);

												auto init_part = new Node();

												auto do_part = get(Node::OPERATOR);

											//#	get(Token(Token::KEYWORD, "finally"), ignore);

												auto finally_part = new Node();

												get(Token(Token::KEYWORD, "else"), ignore);

												auto else_part = get(Node::OPERATOR);

												tmp->children.push_back(else_part);
												tmp->children.push_back(init_part);
												tmp->children.push_back(do_part);
												tmp->children.push_back(finally_part);
												result->children.push_back(tmp);
											//#	return result;
												return tmp;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "finally")) && match(Node::OPERATOR)

								 ) {
													recoil(previousPosition);
												auto tmp = get(Token(Token::KEYWORD, "while"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::EXPRESSION));

												get(Token::BRACE_RIGHT, ignore);

												auto init_part = new Node();

												auto do_part = get(Node::OPERATOR);

												get(Token(Token::KEYWORD, "finally"), ignore);

												auto finally_part = get(Node::OPERATOR);

											//#	get(Token(Token::KEYWORD, "else"), ignore);

												auto else_part = new Node();

												tmp->children.push_back(else_part);
												tmp->children.push_back(init_part);
												tmp->children.push_back(do_part);
												tmp->children.push_back(finally_part);
												result->children.push_back(tmp);
												//#return result;
												return tmp;
										}
									else recoil(previousPosition);



								if(match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) 
								 ) {
														recoil(previousPosition);
												auto tmp = get(Token(Token::KEYWORD, "while"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::EXPRESSION));

												get(Token::BRACE_RIGHT, ignore);

												auto init_part = new Node();

												auto do_part = get(Node::OPERATOR);

											//#	get(Token(Token::KEYWORD, "finally"), ignore);

												auto finally_part = new Node();

											//#	get(Token(Token::KEYWORD, "else"), ignore);

												auto else_part = new Node();

												tmp->children.push_back(else_part);
												tmp->children.push_back(init_part);
												tmp->children.push_back(do_part);
												tmp->children.push_back(finally_part);
												result->children.push_back(tmp);
												//#return result;
												return tmp;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "init")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "finally")) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
												recoil(previousPosition);

												get(Token(Token::KEYWORD, "init"), ignore);
												auto init_part = get(Node::OPERATOR);

												auto tmp = get(Token(Token::KEYWORD, "while"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::EXPRESSION));

												get(Token::BRACE_RIGHT, ignore);

												

												auto do_part = get(Node::OPERATOR);

												get(Token(Token::KEYWORD, "finally"), ignore);

												auto finally_part = get(Node::OPERATOR);

												get(Token(Token::KEYWORD, "else"), ignore);

												auto else_part = get(Node::OPERATOR);

												tmp->children.push_back(else_part);
												tmp->children.push_back(init_part);
												tmp->children.push_back(do_part);
												tmp->children.push_back(finally_part);
												result->children.push_back(tmp);
												//#return result;
												return tmp;
										}
									else recoil(previousPosition);


								if(		match(Token(Token::KEYWORD, "init")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
													recoil(previousPosition);

												get(Token(Token::KEYWORD, "init"), ignore);
												auto init_part = get(Node::OPERATOR);

												auto tmp = get(Token(Token::KEYWORD, "while"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::EXPRESSION));

												get(Token::BRACE_RIGHT, ignore);

												

												auto do_part = get(Node::OPERATOR);

										//#		get(Token(Token::KEYWORD, "finally"), ignore);

												auto finally_part = new Node();

												get(Token(Token::KEYWORD, "else"), ignore);

												auto else_part = get(Node::OPERATOR);

												tmp->children.push_back(else_part);
												tmp->children.push_back(init_part);
												tmp->children.push_back(do_part);
												tmp->children.push_back(finally_part);
												result->children.push_back(tmp);
												//#return result;
												return tmp;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "init")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "finally")) && match(Node::OPERATOR)

								 ) {
													recoil(previousPosition);

												get(Token(Token::KEYWORD, "init"), ignore);
												auto init_part = get(Node::OPERATOR);

												auto tmp = get(Token(Token::KEYWORD, "while"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::EXPRESSION));

												get(Token::BRACE_RIGHT, ignore);

												

												auto do_part = get(Node::OPERATOR);

												get(Token(Token::KEYWORD, "finally"), ignore);

												auto finally_part = get(Node::OPERATOR);

											//#	get(Token(Token::KEYWORD, "else"), ignore);

												auto else_part = new Node();

												tmp->children.push_back(else_part);
												tmp->children.push_back(init_part);
												tmp->children.push_back(do_part);
												tmp->children.push_back(finally_part);
												result->children.push_back(tmp);
												//#return result;
												return tmp;
										}
									else recoil(previousPosition);



								if(		match(Token(Token::KEYWORD, "init")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) && match(Node::OPERATOR) 
								 ) {
													recoil(previousPosition);

												get(Token(Token::KEYWORD, "init"), ignore);
												auto init_part = get(Node::OPERATOR);

												auto tmp = get(Token(Token::KEYWORD, "while"));
												tmp->type = Node::SPECIAL;
												get(Token::BRACE_LEFT, ignore);

												tmp->children.push_back(get(Node::EXPRESSION));

												get(Token::BRACE_RIGHT, ignore);

												

												auto do_part = get(Node::OPERATOR);

												get(Token(Token::KEYWORD, "finally"), ignore);

												auto finally_part = get(Node::OPERATOR);

												get(Token(Token::KEYWORD, "else"), ignore);

												auto else_part = get(Node::OPERATOR);

												tmp->children.push_back(else_part);
												tmp->children.push_back(init_part);
												tmp->children.push_back(do_part);
												tmp->children.push_back(finally_part);
												result->children.push_back(tmp);
												//#return result;
												return tmp;
										}
									else recoil(previousPosition);



								if(		match(Token(Token::KEYWORD, "do")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) &&
								 		match(Token::SEMICOLON)
								 ) {
													recoil(previousPosition);
													auto tmp = get(Token(Token::KEYWORD, "do"));
													tmp->type = Node::SPECIAL;
													tmp->children.push_back(get(Node::OPERATOR));
													get(Token(Token::KEYWORD, "while"), ignore);
													get(Token::BRACE_LEFT, ignore);

													tmp->children.push_back(get(Node::EXPRESSION));

													get(Token::BRACE_RIGHT, ignore);
													get(Token::SEMICOLON, ignore);
													result->children.push_back(tmp);
													//#return result;
													return tmp;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "switch")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) &&
								 		match(Token::CURL_LEFT) && match(Node::CASES) && match(Token::CURL_RIGHT) &&
								 		match(Token(Token::KEYWORD, "finally")) && match(Node::OPERATOR) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)
								 ) {
										recoil(previousPosition);

										auto tmp = get(Token(Token::KEYWORD, "switch"));
										tmp->type = Node::SPECIAL;

										get(Token::BRACE_LEFT, ignore);
										tmp->children.push_back(get(Node::EXPRESSION));
										get(Token::BRACE_RIGHT, ignore);

										get(Token::CURL_LEFT, ignore);
										tmp->children.push_back(get(Node::CASES));
										get(Token::CURL_RIGHT, ignore);

										get(Token(Token::KEYWORD, "finally"), ignore);
										tmp->children.push_back(get(Node::OPERATOR));

										get(Token(Token::KEYWORD, "else"), ignore);
										tmp->children.push_back(get(Node::OPERATOR));

										return tmp;
									}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "switch")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) &&
								 		match(Token::CURL_LEFT) && match(Node::CASES) && match(Token::CURL_RIGHT) &&
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)
								 ) {
										recoil(previousPosition);

										auto tmp = get(Token(Token::KEYWORD, "switch"));
										tmp->type = Node::SPECIAL;

										get(Token::BRACE_LEFT, ignore);
										tmp->children.push_back(get(Node::EXPRESSION));
										get(Token::BRACE_RIGHT, ignore);

										get(Token::CURL_LEFT, ignore);
										tmp->children.push_back(get(Node::CASES));
										get(Token::CURL_RIGHT, ignore);

										tmp->children.push_back(new Node());

										get(Token(Token::KEYWORD, "else"), ignore);
										tmp->children.push_back(get(Node::OPERATOR));

										return tmp;
									}
									else recoil(previousPosition);


								if(		match(Token(Token::KEYWORD, "switch")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT) &&
								 		match(Token::CURL_LEFT) && match(Node::CASES) && match(Token::CURL_RIGHT)

								 ) {
										recoil(previousPosition);

										auto tmp = get(Token(Token::KEYWORD, "switch"));
										tmp->type = Node::SPECIAL;

										get(Token::BRACE_LEFT, ignore);
										tmp->children.push_back(get(Node::EXPRESSION));
										get(Token::BRACE_RIGHT, ignore);

										get(Token::CURL_LEFT, ignore);
										tmp->children.push_back(get(Node::CASES));
										get(Token::CURL_RIGHT, ignore);

										tmp->children.push_back(new Node());

										tmp->children.push_back(new Node());

										return tmp;
										}
									else recoil(previousPosition);

								if(	
										match(Token(Token::KEYWORD, "return")) && 
										match(Node::EXPRESSION) &&
								 		match(Token::SEMICOLON)

								 ) {		
								 			recoil(previousPosition);
											auto tmp = get(Token(Token::KEYWORD, "return"));
											tmp->type = Node::SPECIAL;

											tmp->children.push_back(get(Node::EXPRESSION));

											get(Token::SEMICOLON, ignore);
											return tmp;		
										}
								else recoil(previousPosition);


								if(	
										match(Token(Token::KEYWORD, "return")) && 
								 		match(Token::SEMICOLON)

								 ) {		
								 			recoil(previousPosition);
											auto tmp = get(Token(Token::KEYWORD, "return"));
											tmp->type = Node::SPECIAL;
											tmp->children.push_back(new Node());
											get(Token::SEMICOLON, ignore);
											return tmp;		
										}
								else recoil(previousPosition);

								if(	
										match(Token(Token::KEYWORD, "break")) && 
										match(Node::NAME1) &&
								 		match(Token::SEMICOLON)

								 ) {		recoil(previousPosition);
											auto tmp = get(Token(Token::KEYWORD, "break"));
											tmp->type = Node::SPECIAL;

											tmp->children.push_back(get(Node::NAME1));

											get(Token::SEMICOLON, ignore);
											return tmp;		
										}
								else recoil(previousPosition);

								if(	
										match(Token(Token::KEYWORD, "break")) && 
								 		match(Token::SEMICOLON)

								 ) {	
								 			recoil(previousPosition);
											auto tmp = get(Token(Token::KEYWORD, "break"));
											tmp->type = Node::SPECIAL;

											tmp->children.push_back(new Node());

											get(Token::SEMICOLON, ignore);
											return tmp;		
									}
								else recoil(previousPosition);

								if(	
										match(Token(Token::KEYWORD, "goto")) && 
										match(Node::NAME1) &&
								 		match(Token::SEMICOLON)

								 ) {
								 			recoil(previousPosition);
											auto tmp = get(Token(Token::KEYWORD, "goto"));
											tmp->type = Node::SPECIAL;

											tmp->children.push_back(get(Node::NAME1));

											get(Token::SEMICOLON, ignore);
											return tmp;		
										}
								else recoil(previousPosition);


								} break;



				case Node::CASES: {
										if(match(Node::CASE) && match(Node::CASES)){
													recoil(previousPosition);
													auto tmp = get(Node::CASE);

													result->children = get(Node::CASES)->children; //#!!!!It is not a mistake!
													result->children.insert(result->children.begin(), tmp);
													return result;
										}
										else recoil(previousPosition);

										if(match(Node::CASE)){
												recoil(previousPosition);
												result->children.push_back(get(Node::CASE));
												return result;
										}
										else recoil(previousPosition);

										if(true){
											return result;
										}
										else recoil(previousPosition);


				} break;

				case Node::CASE: {
						if(
							match(Token(Token::KEYWORD, "case")) && match(Node::EXPRESSION) && 
							match(Token(Token::OPERATOR, ":")) &&
							match(Node::OPERATOR) &&
							match(Token(Token::KEYWORD, "continue")) &&  match(Token::SEMICOLON)
						){
							recoil(previousPosition);

							get(Token(Token::KEYWORD, "case"), ignore);

							result->children.push_back(get(Node::EXPRESSION));
							get(Token(Token::OPERATOR, ":"), ignore);

							result->children.push_back(get(Node::OPERATOR));

							result->children.push_back(get(Token(Token::KEYWORD, "continue")));

							get(Token::SEMICOLON, ignore);

							return result;

						}
						else recoil(previousPosition);
										
						if(
							match(Token(Token::KEYWORD, "case")) && match(Node::EXPRESSION) && 
							match(Token(Token::OPERATOR, ":")) &&
							match(Node::OPERATOR) //#No SEMICOLON HERE!
						){
							recoil(previousPosition);

							get(Token(Token::KEYWORD, "case"), ignore);

							result->children.push_back(get(Node::EXPRESSION));
							get(Token(Token::OPERATOR, ":"), ignore);

							result->children.push_back(get(Node::OPERATOR));

							result->children.push_back(new Node());


							return result;
						}
						else recoil(previousPosition);


				} break;

				

				default: throw RecognitionException ("Some trash (" +currentToken.typeToText() + currentToken.getText() + ")on position " + currentToken.position.toString());
			}
			return result;
		
	}


	Parser(const vector<Token> &input){
		logDepth = true;
		depth = 0;
		this->input = input;
		this->currentPosition = 0;
		this->currentToken = Token(Token::BEGIN);
		this->previousPosition = -1;
	}

	void buildTree(){

		log << "PARSING AND VERIFYING \n\n==========\n\n";
		match(Token::BEGIN);

//		match(Node::OPERATORS);
		match(Node::BRACED );

		if(!match(Token::END)){
			throw RecognitionException ("Here should be end of input! " + currentToken.position.toString());	
		}

		log << "PROGRAM IS VALID \n*******************************\n";

		log.flush();


		log << "BUILDING TREE\n\n==========\n\n";
		logDepth = false;
		recoil(0);
		tree = new Node(Node::PROGRAM);
		tree->children.push_back(get(Token::BEGIN));
//		tree->children.push_back( get(Node::OPERATORS));
		tree->children.push_back( get(Node::BRACED));
		tree->children.push_back(get(Token::END));


		log << "BUILT TREE\n\n==========\n\n";


		
	}


};


#endif