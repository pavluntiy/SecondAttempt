#ifndef RECOGNITIONEXCEPTION_H
#define RECOGNITIONEXCEPTION_H
#include <exception>
#include <string>

class RecognitionException : public std::exception
{
public:
	RecognitionException(std::string error)
	{
		this->error = error;
	}

	~RecognitionException() throw()
	{

	}

	const char* what()
	{
		return this->error.c_str();
	}

	std::string error;
};
	
#endif