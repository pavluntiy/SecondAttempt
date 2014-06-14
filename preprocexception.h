#ifndef PREPROCEXCEPTION_H 
#define PREPROCEXCEPTION_H 

#include <exception>
#include <string>

class PreprocessorException : public std::exception
{
public:
	PreprocessorException(std::string error)
	{
		this->error = error;
	}

	PreprocessorException() throw()
	{

	}

	const char* what()
	{
		return this->error.c_str();
	}

	std::string error;
};

#endif 