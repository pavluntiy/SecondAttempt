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
		FORTHING
	}; 

	NodeType type;
	string text;
	vector<Node*> children;
	Position position;

	Node (NodeType type, vector<Node*> children){
		this->type = type;
		this->children = children;


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
			case CONDITION: 	return "CONDITION";
			case EXPRESSION: 	return "EXPRESSION";
			case VARDEF:		return "VARDEF";
			case CASES:		return "CASES";
			case CASE: 		return "CASE";	
			case ATOMIC_CONDITION:	return "ATOMIC_CONDITION";
			case COMPARISION: 		return "COMPARISION";
			case IS_IN_EXPRESSION: 	return "IS_IN_EXPRESSION";
			case AND_COND: return "AND_COND";
			case FUNCALL: 		return "FUNCALL";
			case NAME: 			return "NAME";	
			case NAME1: 			return "NAME1";	
			case NAMESPACE: 			return "NAMESPACE";	
			case ASSIGNMENT: 	return "ASSIGNMENT";
			case ARGLIST: 		return "ARGLIST";
			case LIST: 			return "LIST";
			case LIST_ELEM: return "LIST_ELEM";
			case VALUE: 		return "VALUE";
			case CONSTANT: 		return "CONSTANT";
			case ID: 			return "ID";
			case EXPR1: 		return "EXPR1";
			case EXPR2: 		return "EXPR2";
			case EXPR3: 		return "EXPR3";
			case EXPR4: 		return "EXPR4";
			case EXPR5:			return "EXPR5";
			case EXPR6:			return "EXPR6";

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
		}
	}

	bool operator == (const Node &other){
		return this->type == other.type;
	}

	bool operator != (const Node &other){
		return this->type != other.type;
	}

	bool strictEq (const Node &other){
		return this->type == other.type &&  this->text == other.text;
	}

};


#endif