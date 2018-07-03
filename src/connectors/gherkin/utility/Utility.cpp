#include "Utility.h"

std::string narrowString(const std::wstring& wideString)
{
    return std::string(wideString.begin(), wideString.end());
}
