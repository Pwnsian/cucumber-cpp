#include <cucumber-cpp/internal/connectors/gherkin/utility/Utility.hpp>

namespace cucumber {
namespace internal {

std::string narrowString(const std::wstring& wideString)
{
    return std::string(wideString.begin(), wideString.end());
}

}
}
