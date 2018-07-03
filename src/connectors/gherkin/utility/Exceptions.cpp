#include <string>
#include "exceptions.h"

StringException::StringException(const std::string& message)
    : m_Message(message)
{

}

const char* StringException::what() const noexcept
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
