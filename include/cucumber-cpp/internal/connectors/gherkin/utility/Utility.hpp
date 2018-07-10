#ifndef UTILITY_H
#define UTILITY_H
#include <cucumber-cpp/internal/connectors/gherkin/utility/Types.hpp>

namespace cucumber {
namespace internal {

std::string narrowString(const std::wstring& wideString);

template<typename T>
void replaceAll(
    std::basic_string<T>& str,
    const std::basic_string<T>& from,
    const std::basic_string<T>& to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

}
}

#endif