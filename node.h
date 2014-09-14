#ifndef NODE_H
#define NODE_H

#include "token.h"

#include <map>
#include <string>


class Node : public Token {

public: 
	enum NodeType {
		NONE, 
		PROGRAM, 
		ACTION, 
		BODY, 
		OPERATOR, 
		OPERATORS, 
		SPECIAL, 
		DECLARATION, 
		VARLIST,
		CONDITION, 
		EXPRESSION, 
		VARDEF, 
		CASES, 
		CASE, 
		ATOMIC_CONDITION,
		COMPARISION, 
		IS_IN_EXPRESSION, 
		AND_COND,
		FUNCALL, 
		NAME, 
		NAME1,
		NAMESPACE,
		ASSIGNMENT, 
		ARGLIST, 
		LIST, 
		LIST_ELEM,
		VALUE, 
		CONSTANT, 
		ID, 
		EXPR1, 
		EXPR2, 
		EXPR3, 
		EXPR4, 
		EXPR5,
		EXPR6,
		EXPR7,
		EXPR8,
		BRACED,
		ACCESS,
		ACCESS_EXPR,
		L_OR,
		L_AND,
		B_OR,
		B_XOR,
		B_AND,
		TYPE, 
		VARQ, 
		TYPEDEF, 
		PREDEF_TYPE,
		FUNCDEF,
		DECL_LIST,
		DECL_ATOM,
		FORTHING,

		INT, 
		FLOAT, 
		CHAR, 
		BOOL, 
		STRING,
		BEGIN, END, BLOCK_BEGIN, 
					BLOCK_END, COMMENT, DIRECT, SEMICOLON, 
					BRACE_LEFT, BRACE_RIGHT,
					BRACKET_LEFT, BRACKET_RIGHT,
					CURL_LEFT, CURL_RIGHT,
					KEYWORD, 
					COMMA, DOT,
					ASSIGN
	}; 

	NodeType type;
	string text;
	vector<Node*> children;
	Position position;


	NodeType convert(Token::Type type){
		switch (type){
			case Token::NONE: return NONE;
			case Token::BEGIN: return BEGIN;
			case Token::END: return END;
			case Token::BLOCK_BEGIN: return BLOCK_BEGIN;
			case Token::BLOCK_END: return BLOCK_END;
			case Token::COMMENT: return COMMENT;
			case Token::DIRECT: return DIRECT;
			case Token::SEMICOLON: return SEMICOLON;
			case Token::NAME: return NAME;
			case Token::OPERATOR: return OPERATOR;
			case Token::BRACE_RIGHT: return BRACE_RIGHT;
			case Token::BRACE_LEFT: return BRACE_LEFT;
			case Token::BRACKET_RIGHT: return BRACKET_RIGHT;
			case Token::BRACKET_LEFT: return BRACKET_LEFT;
			case Token::CURL_RIGHT: return CURL_RIGHT;
			case Token::CURL_LEFT: return CURL_LEFT;
			case Token::KEYWORD: return KEYWORD;
			case Token::COMMA: return COMMA;
			case Token::ASSIGN: return ASSIGN;
			case Token::DOT: return DOT;
			case Token::INT: return INT;
			case Token::FLOAT: return FLOAT;
			case Token::CHAR: return CHAR;
			case Token::BOOL: return BOOL;
			case Token::STRING: return STRING;
		}
	}



	Node(const Token &token){
		this->type = convert(token.type);
		this->text = token.text;
	}

	Node (NodeType type = NONE){
		this->type = type;
	}

	~Node(){

	}

	std::string typeToText(){

		switch(this->type){
			case NONE:		return "NONE";
			case PROGRAM:	return "PROGRAM";
			case ACTION: 	return "ACTION";
			case BODY: 		return "BODY";
			case OPERATOR: 	return "OPERATOR";
			case OPERATORS: return "OPERATORS";
			case SPECIAL: 	return "SPECIAL";
			case DECLARATION: return "DECLARATION";
			case VARLIST: return "VARLIST";
			case CONDITION: 	return "CONDITION";
			case EXPRESSION: 	return "EXPRESSION";
			case VARDEF:		return "VARDEF";
			case CASES:			return "CASES";
			case CASE: 			return "CASE";	
			case ATOMIC_CONDITION:	return "ATOMIC_CONDITION";
			case COMPARISION: 		return "COMPARISION";
			case IS_IN_EXPRESSION: 	return "IS_IN_EXPRESSION";
			case AND_COND: 		return "AND_COND";
			case FUNCALL: 		return "FUNCALL";
			case NAME: 			return "NAME";	
			case NAME1: 		return "NAME1";	
			case NAMESPACE: 	return "NAMESPACE";	
			case ASSIGNMENT: 	return "ASSIGNMENT";
			case ARGLIST: 		return "ARGLIST";
			case LIST: 			return "LIST";
			case LIST_ELEM: 	return "LIST_ELEM";
			case VALUE: 		return "VALUE";
			case CONSTANT: 		return "CONSTANT";
			case ID: 			return "ID";
			case EXPR1: 		return "EXPR1";
			case EXPR2: 		return "EXPR2";
			case EXPR3: 		return "EXPR3";
			case EXPR4: 		return "EXPR4";
			case EXPR5:			return "EXPR5";
			case EXPR6:			return "EXPR6";
			case EXPR7:			return "EXPR7";
			case EXPR8:			return "EXPR8";
			case BRACED:		return "BRACED";
			case ACCESS:		return "ACCESS";
			case ACCESS_EXPR:	return "ACCESS_EXPR";

			case L_OR:			return "L_OR";
			case L_AND:			return "L_AND";

			case B_OR:			return "B_OR";
			case B_XOR:			return "B_XOR";
			case B_AND:			return "B_AND";


			case TYPE: 			return "TYPE";
			case VARQ: 			return "VARQ";
			case TYPEDEF: 		return "TYPEDEF";
			case PREDEF_TYPE:	return "PREDEF_TYPE";
			case FUNCDEF:	return "FUNCDEF";
			case DECL_LIST:	return "DECL_LIST";
			case DECL_ATOM:	return "DECL_ATOM";
			case FORTHING: return "FORTHING";

			case INT: return "INT";
			case FLOAT: return "FLOAT";
			case CHAR:  return "CHAR";
			case BOOL: 	return "BOOL";
			case STRING: return "STRING";

			case BEGIN: return "BEGIN";
			case END: return "END";
			case BLOCK_BEGIN: return "BLOCK_BEGIN";
			case BLOCK_END: return "BLOCK_END";
			case COMMENT: return "COMMENT";
			case DIRECT: return "DIRECT";
			case SEMICOLON: return "SEMICOLON";
			case BRACKET_LEFT: return "BRACKET_LEFT";
			case BRACKET_RIGHT: return "BRACKET_RIGHT";
			case BRACE_LEFT: return "BRACE_LEFT";
			case BRACE_RIGHT: return "BRACE_RIGHT";
			case CURL_LEFT: return "CURL_LEFT";
			case CURL_RIGHT: return "CURL_RIGHT";
			case KEYWORD: return "KEYWORD";
			case COMMA: return "COMMA";
			case DOT: return "DOT";
		}
	}

	bool operator == (const Node &other)
	const{
		return this->type == other.type;
	}

	bool operator != (const Node &other)
	const{
		return this->type != other.type;
	}

	bool strictEq (const Node &other)
	const{
		return this->type == other.type &&  this->text == other.text;
	}

};

/*
vector<Node*> operator && (Node *first, Node *second){

	if(*first == Node() && *second == Node()){
		return vector<Node*>(1, new Node(Node::NONE));
	}

	if(*first == Node()){
		return vector<Node*>(1, second);
	}

	if(*second == Node()){
		return vector<Node*>(1, first);
	}

	vector<Node*> result;
	result.push_back(first);
	result.push_back(second);

	return result;

}

/*vector<Node*> operator && (const vector<Node*> &array, Node *second){
	if(array.size() == 1 && *array[0] == Node()){
		return vector<Node*>(1, second);
	}

	if(*second == Node()){
		return array;
	}

	auto result = array;
	result.push_back(second);

	return result;
}

/*vector<Node>& operator = (vector<Node> &array, Node &node){
	array.push_back(node);
	return array;
}
*/

#endif