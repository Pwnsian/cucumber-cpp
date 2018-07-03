#ifndef _GHERKINCPP_EXCEPTION_H
#define _GHERKINCPP_EXCEPTION_H

#include <string>
#include <exception>

class StringException : public std::exception
{
public:
    StringException(const std::string& message);
    virtual const char* what() const noexcept override;
protected:
    std::string m_Message;
};

class FileNotFoundException : public StringException
{
public:
    FileNotFoundException(const std::string& message);
};

class ParserErrorException : public StringException
{
public:
    ParserErrorException(const std::string& message);
};

#endif