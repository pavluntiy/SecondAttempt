#ifndef PARSEREXCEPTION_H 
#define PARSEREXCEPTION_H 

#include <exception>
#include <string>

class ParserException : public std::exception
{
public:
	ParserException(std::string error)
	{
		this->error = error;
	}

	~ParserException() throw()
	{

	}

	const char* what()
	{
		return this->error.c_str();
	}

	std::string error;
};

#endif 
