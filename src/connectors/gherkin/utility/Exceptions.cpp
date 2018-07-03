#include <string>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Exceptions.hpp>

namespace cucumber {
namespace internal {

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

}
}
