#include <string>
#include <string>
#include <sstream>
#include <iostream>
#include <gherkin-c/include/string_token_scanner.h>
#include <gherkin-c/include/file_reader.h>
#include <gherkin-c/include/token_matcher.h>
#include <gherkin-c/include/ast_builder.h>
#include <gherkin-c/include/parser.h>
#include <gherkin-c/include/error.h>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Exceptions.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Utility.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/parser/GherkinParser.hpp>

namespace cucumber {
namespace internal {

GherkinParser::GherkinParser(const std::wstring& featureContents)
    : m_featureContents(featureContents)
    , m_tokenScanner(NULL)
    , m_tokenMatcher(NULL)
    , m_bulder(NULL)
    , m_parser(NULL)
{
    // Workaround for Gherkin-C not properly parsing files indented with tabs.
    replaceAll<wchar_t>(m_featureContents, L"\t", L" ");

    m_tokenScanner = StringTokenScanner_new(m_featureContents.c_str());
    m_tokenMatcher = TokenMatcher_new(L"en");
    m_bulder = AstBuilder_new();
    m_parser = Parser_new(m_bulder);
}

GherkinParser::~GherkinParser()
{
    TokenScanner_delete(m_tokenScanner);
    TokenMatcher_delete(m_tokenMatcher);
    AstBuilder_delete(m_bulder);
    Parser_delete(m_parser);
}

std::wstring GherkinParser::loadFeatureFile(const std::string& filename)
{
    FileReader* fileReader = FileReader_new(filename.c_str());

    // Gherkin-C's strange method of handling file not found. Handle it.
    // https://github.com/cucumber/gherkin-c/commit/db6ae6d2b99e68df47d774aac35f35109dc807a1
    std::wstring fileContents = FileReader_read(fileReader);
    if(fileContents == L" ")
    {
        FileReader_delete(fileReader);
        throw FileNotFoundException(filename);
    }

    FileReader_delete(fileReader);
    return fileContents;
}

std::string GherkinParser::ReadParserErrors(Parser* parser)
{
    std::wstringstream ss;

    while(Parser_has_more_errors(parser))
    {
        Error* error = Parser_next_error(parser);
        Location location = error->location;
        ss << error->error_text << " at " << location.line << ":" << location.column << std::endl;
        Error_delete(error);
    }

    std::wstring allErrors = ss.str();
    std::string narrowedErrors(allErrors.begin(), allErrors.end());

    return narrowedErrors;
}

GherkinDocumentPtr GherkinParser::parse()
{
    int result = Parser_parse(m_parser, m_tokenMatcher, m_tokenScanner);

    if(result != 0)
    {
        std::string errors = ReadParserErrors(m_parser);
        throw ParserErrorException(errors);
    }

    // TODO: Check other errors not reported by parser:
    // * If a Scenario Outline is missing examples

    return GherkinDocumentPtr(AstBuilder_get_result(m_bulder));
}

}
}