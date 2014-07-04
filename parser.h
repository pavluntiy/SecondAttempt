#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include "recognitionexception.h"

#include "node.h"
//#include "tree.h"

const bool ignore = true;

class Parser {
public:
	bool dontConsume = false;
	vector<Token> input;
	Node *tree;
	int currentPosition, previousPosition;
	Token currentToken;
	map<pair<Node::NodeType, int>, pair<bool, int> > memo;

	void recoil(int previousPosition = 0){
		cout << "\t=>Recoiled from " << currentPosition << " to " << previousPosition << "; ";
		currentPosition = previousPosition;
		currentToken = input[currentPosition];
		cout << currentToken.typeToText() << "\n";
	}

	void consume(bool stop = false){
		if(stop){
			return;
		}
		currentPosition++;
		if(currentPosition < input.size())
				currentToken = input[currentPosition];
			else{
				currentToken = Token(Token::END);
			}
		while(currentToken == Token(Token::BEGIN) ||
			currentToken == Token(Token::COMMENT) || currentToken == Token(Token::DIRECT)
		){
			currentPosition++;
			if(currentPosition < input.size())
				currentToken = input[currentPosition];
			else{
				currentToken = Token(Token::END);
			}
		}
	}

	bool match(Token::Type type){
			cout << "\t Trying " << Token(type).typeToText()<< "\n";
		if(type == currentToken.type){
			cout << "\t Found " << currentToken.typeToText()<< "\n";
			consume(dontConsume);
			return true;
		}else {
			if(type == Token::CURL_RIGHT || type == Token::BRACE_RIGHT){
				cout << ("\t\tYou could have missed " + typeToText(type) + " on position " + currentToken.position.toString() + " (Token # " + std::to_string(currentPosition) +")");
			}

			cout << "\t Tried " << Token(type).typeToText() << ", got " << currentToken.typeToText() << "\n";
			return false;
		}
	}
	bool match(Token token){
		cout << "\t\tTrying " << token.typeToText() << token.getText() << "\n";
		if(currentToken.type == token.type && currentToken.text == token.text){
			cout << "\t\tFound " << token.typeToText() << token.getText() << "\n";
			consume(dontConsume);
			return true;
		}
		

		if(token.text == "" && currentToken.type == token.type){

			consume(dontConsume);
			return true;
		}



					cout << "\t\tFailed " << currentToken.typeToText() << currentToken.getText() << "\n";
		return false;
	}

	bool match(Node::NodeType what){

		cout << "Looking for " << Node(what).typeToText() << " on position "  << currentPosition << "\n";
		int previousPosition = currentPosition;
		bool result = false;
		if(memo.count(make_pair(what, currentPosition))){	
			result =  memo[make_pair(what, currentPosition)].first;
			cout << "\t\t!!!!!!Got in memo: " << Node(what).typeToText() << " on position "  << currentPosition << " is " << result << "\n\t\t\t";
			if(result){
				cout << "previous position = " << currentPosition << " ";
				recoil(memo[make_pair(what, currentPosition)].second);
				cout << "skipped to position " << currentPosition << "\n";
			}
			
			
		}
		else{
			switch(what){
				case Node::VALUE: {				
												if(match(Node::NAME) && match(Token::BRACKET_LEFT) && match(Node::EXPRESSION) && match(Token::BRACKET_RIGHT)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL) && match(Token::BRACKET_LEFT) && match(Node::EXPRESSION) && match(Token::BRACKET_RIGHT)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::ARGLIST)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL) && match(Token::DOT) && match(Node::VALUE)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL) && match(Token(Token::OPERATOR, "->")) && match(Node::VALUE)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL)){
													result = true;
													break;
												}
												else recoil(previousPosition);

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

				case Node::EXPR6: 			{
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

				case Node::EXPR5: 			{										
												if(match(Node::EXPR6) && match(Token(Token::OPERATOR, "**")) && match(Node::EXPR6)){
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
												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "==")) && match(Node::B_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, ">=")) && match(Node::B_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "<=")) && match(Node::B_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "!=")) && match(Node::B_OR)){
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
										if(match(Node::COMPARISION) && match(Token(Token::KEYWORD, "is")) && match(Node::COMPARISION)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::COMPARISION) && match(Token(Token::KEYWORD, "in")) && match(Node::COMPARISION)){
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


/*					case Node::CONDITION: {										
												if(match(Token(Token::OPERATOR, "!")) && match(Node::CONDITION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::IS_IN_EXPRESSION)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::ATOMIC_CONDITION) && match(Token(Token::OPERATOR, "||")) && match(Node::CONDITION)){
													result = true;
													break;
												}
												else recoil(previousPosition);


												if(match(Node::ATOMIC_CONDITION)){
													result = true;
													break;
												}
												else recoil(previousPosition);
											} break;

					case Node::ATOMIC_CONDITION: {		

												if(match(Node::AND_COND)){
													result = true;
													break;
												}
												else recoil(previousPosition);
												

												if(match(Token::BRACE_LEFT) && match(Node::CONDITION) && match(Token::BRACE_RIGHT)){
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

					case Node::AND_COND: {
												if(match(Node::COMPARISION) && match(Token(Token::OPERATOR, "&&")) && match(Node::AND_COND)){
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
*/


					case Node::EXPRESSION: {			
									//			if(match(Node::CONDITION)){
									//				result = true;
									//				break;
									//			}
									//			else recoil(previousPosition);
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
									//			if(match(Node::FUNCALL)){
									//				result = true;
									//				break;
									//			}
									//			else recoil(previousPosition);
							
												

												

												
											} break;

					case Node::ASSIGNMENT: {		

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "+=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "-=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "/=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "%=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "*=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "|=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "||=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "&=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "&&=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

//												if(match(Token::NAME) && match(Token(Token::OPERATOR, "=")) && match(Node::VALUE)){
//													result = true;
//													break;
//												}
//												else recoil(previousPosition);

											} break;




					case Node::FUNCALL: {

								//		if(match(Node::FUNCALL) && match(Node::ARGLIST)){
								//					result = true;
								//					break;
								//		}
								//		else recoil(previousPosition);

										if(match(Node::NAME) && match(Node::ARGLIST)){
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

										if(match(Token(Token::OPERATOR, "*")) && match(Node::NAME1)){
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



							//			if(true){
							//					result = true;
							//					break;
							//			}
							//			else recoil(previousPosition);





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

										if(match(Node::EXPR1)){
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
										if(match(Node::TYPE) &&  match(Node::LIST)) {
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
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "do")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT)
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
							match(Node::OPERATOR)
						){
									result = true;
									break;
						}
						else recoil(previousPosition);


				} break;









				default: throw RecognitionException ("Some trash (" +currentToken.typeToText() + currentToken.getText() + ")on position " + currentToken.position.toString());
			}
		}
		cout << "Result for " << Node(what).typeToText() << " on position "  << previousPosition << " is " << result<<"\n";
		cout << "\tNow on " << currentPosition << "\n";
		memo[make_pair(what, previousPosition)] = make_pair(result, currentPosition);

		if(result == false){
			recoil(previousPosition);
		}
		return result;

	} 

	Node* get(const Token &what, bool ignore = false){
		

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
	//		auto res = new Node(result);
	//		cout << "Node text: " << res->text << "\n\n";
			return new Node(result);
		}
		else {
			throw RecognitionException("Error in building tree on position " + to_string(currentPosition) + " (you should check what you are trying to get)");
		}

	}

	Node* get(Node::NodeType what, bool ignore = false){
				int previousPosition = currentPosition;
				cout << previousPosition << "(prev pos)";
				Node *result = new Node(what);
				switch(what){
				case Node::VALUE: {				
							/*					if(match(Node::NAME) && match(Token::BRACKET_LEFT) && match(Node::EXPRESSION) && match(Token::BRACKET_RIGHT)){
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
													recoil(previousPosition);
													result->children.push_back(get(Node::ARGLIST));
													return result;
													
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL) && match(Token::DOT) && match(Node::VALUE)){
													recoil(previousPosition);
													auto tmp = get(Node::FUNCALL);
													auto root = get(Token::DOT);
													root->children.push_back(tmp);
													root->children.push_back(get(Node::VALUE));
													result->children.push_back(root);
													return result;
												
												}
												else recoil(previousPosition);

												if(match(Node::FUNCALL) && match(Token(Token::OPERATOR, "->")) && match(Node::VALUE)){
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

												if(match(Token::INT)){
													recoil(previousPosition);
													result->children.push_back(get(Token::INT));
													return result;
												}
												else recoil(previousPosition);
//												cout << "!!!!!!!!!!!!!!!!FAILED INT\n";
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

/*				case Node::EXPR6: 			{
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

				case Node::EXPR5: 			{										
												if(match(Node::EXPR6) && match(Token(Token::OPERATOR, "**")) && match(Node::EXPR6)){
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
												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "==")) && match(Node::B_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, ">=")) && match(Node::B_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "<=")) && match(Node::B_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Node::B_OR) && match(Token(Token::OPERATOR, "!=")) && match(Node::B_OR)){
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
										if(match(Node::COMPARISION) && match(Token(Token::KEYWORD, "is")) && match(Node::COMPARISION)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::COMPARISION) && match(Token(Token::KEYWORD, "in")) && match(Node::COMPARISION)){
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

*/
					case Node::EXPRESSION: {			
								/*				if(match(Node::ASSIGNMENT)){
													result = true;
													break;
												}
												else recoil(previousPosition);
								
												if(match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);
								*/

								//temporary measure until I enable sterling expressions

												if(match(Node::VALUE)){
													recoil(previousPosition);
													result->children.push_back(get(Node::VALUE));
													return result;
												}	


											} break;
/*
					case Node::ASSIGNMENT: {		

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "+=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "-=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "/=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "%=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "*=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "|=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "||=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "&=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

												if(match(Token::NAME) && match(Token(Token::OPERATOR, "&&=")) && match(Node::L_OR)){
													result = true;
													break;
												}
												else recoil(previousPosition);

//												if(match(Token::NAME) && match(Token(Token::OPERATOR, "=")) && match(Node::VALUE)){
//													result = true;
//													break;
//												}
//												else recoil(previousPosition);

											} break;


*/

					case Node::FUNCALL: {
										if(match(Node::NAME) && match(Node::ARGLIST)){
											recoil(previousPosition);
											result->children.push_back(get(Node::NAME));
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
										
									
										if(match(Token(Token::OPERATOR, "*")) && match(Node::NAME1)){
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
										

										if(match(Node::NAME1)){
												recoil(previousPosition);
												return get(Node::NAME1);
										}
										else recoil(previousPosition);



										} break;

					case Node::NAME1: {	

	
											if(match(Token::NAME)){
								
												recoil(previousPosition);											
												result->children.push_back(get(Token::NAME));
												return result;
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
													get(Token::COMMA);
													result->children = get(Node::LIST)->children; //!!!!It is not a mistake!
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

				/*						if(match(Node::ASSIGNMENT)){
													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::EXPR1)){
													result = true;
													break;
										}
										else recoil(previousPosition);
				*/

										

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
												//	cout << "All's right!\n\n";
													return result;
									}
									else recoil(previousPosition);

									} break;

				
									
					case Node::OPERATOR: {
						/*				if(match(Node::SPECIAL)){
													result = true;
													break;
										}
										else recoil(previousPosition);
										if(match(Node::FUNCDEF)){
d													result = true;
													break;
										}
										else recoil(previousPosition);

										if(match(Node::DECLARATION) && match(Token::SEMICOLON)){
													result = true;
													break;
										}
										else recoil(previousPosition);
*/
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
													result->children = get(Node::OPERATORS)->children; //!!!!It is not a mistake!
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

		/*		case Node::DECLARATION: {
										if(match(Node::TYPE) &&  match(Node::LIST)) {
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
								 		match(Token(Token::KEYWORD, "else")) && match(Node::OPERATOR)

								 ) {
													result = true;
													break;
										}
									else recoil(previousPosition);

								if(		match(Token(Token::KEYWORD, "do")) && match(Node::OPERATOR) &&
										match(Token(Token::KEYWORD, "while")) && match(Token::BRACE_LEFT) &&
										match(Node::EXPRESSION) &&
								 		match(Token::BRACE_RIGHT)
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
							match(Node::OPERATOR)
						){
									result = true;
									break;
						}
						else recoil(previousPosition);


				} break;

				*/

				default: throw RecognitionException ("Some trash (" +currentToken.typeToText() + currentToken.getText() + ")on position " + currentToken.position.toString());
			}
			//cout << "this has " << result->children.size() <<'\n';
			auto shift = " ";
	//		cout << shift << "( " << result->typeToText() << ", text" << result->text << ", children num = "<< result->children.size() << ":\n";
	//		cout << shift << "( " << result->children[0]->typeToText() << ", text" << result->children[0]->text << ", children num = "<< result->children[0]->children.size() << ":\n";
	//		cout << shift << ")\n";	
	//		cout << shift << ")\n";
			return result;
		
	}


	Parser(const vector<Token> &input){
		this->input = input;
		this->currentPosition = 0;
		this->currentToken = Token(Token::BEGIN);
		this->previousPosition = -1;
	}

	void buildTree(){
		tree = new Node(Node::PROGRAM);
		match(Token::BEGIN);

		match(Node::OPERATORS);
	//	match(Node::VALUE);

		cout << currentToken.text;
		if(!match(Token::END)){
			throw RecognitionException ("Here should be end of input! " + currentToken.position.toString());	
		}

		recoil(0);
	//	dontConsume = true;
		cout << "BUILDING TREE\n\n==========\n\n";
		tree->children.push_back(get(Token::BEGIN));
		tree->children.push_back( get(Node::OPERATORS));
//			tree.children[1]->children.push_back(new Node());
		tree->children.push_back(get(Token::END));

	//	dfs(*tree.children[1]->children[0]);
	//	dfs(*tree.children[0]);
		cout << "BUILT TREE\n\n==========\n\n";
//		dfs(tree);

		
	}

void dfs (Node *node, string shift = ""){
	cout << shift << "( " << node->typeToText() << ", text: " << node->text << ", children num = "<< node->children.size() << ":\n";
		for(int i = 0; i < node->children.size(); ++i){
			dfs(node->children[i], shift + ' ');
		}
	cout << shift << ")\n";
}
};


#endif