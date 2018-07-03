#ifndef GHERKIN_PARSER_H
#define GHERKIN_PARSER_H

#include "../utility/Types.hpp"

namespace cucumber {
namespace internal {

class GherkinParser
{
public:
    GherkinParser(const std::string& filename);
    ~GherkinParser();
    GherkinDocumentPtr parse();

private:
    std::string ReadParserErrors(Parser* parser);

private:
    FileReader* m_fileReader;
    TokenScanner* m_tokenScanner;
    TokenMatcher* m_tokenMatcher;
    Builder* m_bulder;
    Parser* m_parser;    
};

}
}

#endif