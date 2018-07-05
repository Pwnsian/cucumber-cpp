#include <string>
#include <sstream>
#include <gherkin-c/include/step.h>
#include <gherkin-c/include/scenario.h>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Exceptions.hpp>

namespace cucumber {
namespace internal {

StringException::StringException()
    : m_Message("")
{

}

StringException::StringException(const std::string& message)
    : m_Message(message)
{

}

StringException::~StringException() throw()
{
    
}

const char* StringException::what() const throw()
{
    return m_Message.c_str();
}

FileNotFoundException::FileNotFoundException(const std::string& filename)
    : StringException("File not found : " + filename)
{

}

ParserErrorException::ParserErrorException(const std::string& errors)
    : StringException("Parser errors : " + errors)
{

}

NotImplementedException::NotImplementedException(const std::string& message)
    : StringException(message + " are currently not implemented.")
{

}

MissingStepDefinitionException::MissingStepDefinitionException(
    const std::string& targetStepName, const std::string& snippetText)
{
    std::stringstream ss;
    ss << "No matching step definition found for '" << targetStepName << "'. " << std::endl;
    ss << "It can be implemented with the following: " << std::endl;
    ss << snippetText << std::endl;
    m_Message = ss.str();
}

AmbiguousStepDefinitionException::AmbiguousStepDefinitionException(
    const std::string& targetStepName, const std::vector<StepMatch>& foundMatches)
{
    std::stringstream ss;
    ss  << "Ambiguous step definitions found for '" << targetStepName << "':" << std::endl;
    for(std::vector<StepMatch>::const_iterator it = foundMatches.begin(); it != foundMatches.end(); ++it)
    {
        ss << " -> " << printStepDefinition(*it) << std::endl;
    }
    m_Message = ss.str();
}

std::string AmbiguousStepDefinitionException::printStepDefinition(const StepMatch& step)
{
    std::stringstream ss;
    ss << step.source << " " << step.regexp;
    return ss.str();
}

}
}
