#ifndef _GHERKINCPP_EXCEPTION_H
#define _GHERKINCPP_EXCEPTION_H

#include <string>
#include <vector>
#include <exception>
#include <cucumber-cpp/internal/CukeEngine.hpp>

namespace cucumber {
namespace internal {

class StringException : public std::exception
{
public:
    StringException();
    StringException(const std::string& message);
    virtual ~StringException() throw ();
    virtual const char* what() const throw ();
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

class NotImplementedException : public StringException
{
public:
    NotImplementedException(const std::string& message);
};

class MissingStepDefinitionException : public StringException
{
public:
    MissingStepDefinitionException(
        const std::string& targetStepName,
        const std::string& snippetText
    );
};

class AmbiguousStepDefinitionException : public StringException
{
public:
    AmbiguousStepDefinitionException(
        const std::string& targetStepName, 
        const std::vector<StepMatch>& foundMatches);
private:
    std::string printStepDefinition(const StepMatch& step);
};

}
}

#endif