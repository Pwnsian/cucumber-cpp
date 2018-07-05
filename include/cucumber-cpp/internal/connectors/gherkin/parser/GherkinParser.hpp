#ifndef GHERKIN_PARSER_H
#define GHERKIN_PARSER_H

#include "../utility/Types.hpp"

namespace cucumber {
namespace internal {

class GherkinParser
{
public:
    GherkinParser(const std::wstring& featureContents);
    ~GherkinParser();

    GherkinDocumentPtr parse();
    static std::wstring loadFeatureFile(const std::string& filename);

private:
    std::string ReadParserErrors(Parser* parser);

private:
    std::wstring m_featureContents;
    TokenScanner* m_tokenScanner;
    TokenMatcher* m_tokenMatcher;
    Builder* m_bulder;
    Parser* m_parser;    
};

}
}

#endif