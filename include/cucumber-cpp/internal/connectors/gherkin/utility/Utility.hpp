#ifndef UTILITY_H
#define UTILITY_H
#include <cucumber-cpp/internal/connectors/gherkin/utility/Types.hpp>

namespace cucumber {
namespace internal {

std::string narrowString(const std::wstring& wideString);
void replaceAll(std::string& str, const std::string& from, const std::string& to);

}
}

#endif