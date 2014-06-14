#ifndef ALPHABET_H
#define ALPHABET_H
class Alphabet {
public:
	static bool isIdCharacter(char c){
		return isLetter(c) || isDigit(c);
	}
	static	bool isLetter(char c){
	        return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_');
	}

	static bool isDigit(char c){
		return ('0' <= c && c <= '9');
	}

	static bool isBinaryDigit(char c){
		return ('0' <= c && c <= '1');
	}

	static bool isOctalDigit(char c){
		return ('0' <= c && c <= '7');
	}

	static bool isHexadecimalDigit(char c){
		return (('0' <= c && c <= '9') || ('A' <= c && c <= 'F') ||('a' <= c && c <= 'f'));
	}

	static bool isWhitespace(char c){
		return (c == ' ' || c == '\r');
	}

	static bool isNewline(char c){
		return (c == '\n');
	}

	static bool isTab(char c){
		return (c == '\t');
	} 

	static bool isIdle(char c){
		return isTab(c) || isNewline(c) || isWhitespace(c);
	}

	static bool isEof(char c){
	        return (c == EOF);
	}

	static bool isSeparator (char c){
		return (isTab(c) || isNewline(c) || isWhitespace(c) || isEof(c));
	}

	static bool isCommentSign (char c){
		return (c == '#');
	}

	static bool isSpecialSymbol (char c){
		bool isTrue = false;

		isTrue = isTrue || (c == ';');
		isTrue = isTrue || (c == '[' || c ==']');
		isTrue = isTrue || (c == '+'|| c == '/' || c == '*' || c == '%' || c == '-');
		isTrue = isTrue || (c == '&' || c == '|' || c == '^' || c == '~' || c == '!');
		isTrue = isTrue || (c == ',' || c == '.');
		isTrue = isTrue || (c == '<' || c == '>');
		isTrue = isTrue || (c == '{' || c == '}');
		isTrue = isTrue || (c == '?' || c == ':');
		isTrue = isTrue || (c == '\\' || c == '\'' || c == '\"');
		isTrue = isTrue || (c == '@' || c == '$');

		return isTrue;

	}

	static bool isAcceptableCharacter (char c){
		bool isTrue = false;
		isTrue = isTrue || isNewline(c);
		isTrue = isTrue || isWhitespace(c);
		isTrue = isTrue || isCommentSign(c);
		isTrue = isTrue || isSpecialSymbol(c);
		isTrue = isTrue || isTab(c);
		isTrue = isTrue || isLetter(c);
		isTrue = isTrue || isDigit(c);
		return isTrue;
	}
};

#endif