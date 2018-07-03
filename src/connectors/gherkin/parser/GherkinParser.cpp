#include <string>
#include <string>
#include <sstream>
#include "gherkin-c/include/string_token_scanner.h"
#include "gherkin-c/include/file_reader.h"
#include "gherkin-c/include/token_matcher.h"
#include "gherkin-c/include/ast_builder.h"
#include "gherkin-c/include/parser.h"
#include "gherkin-c/include/error.h"
#include "utility/Exceptions.hpp"
#include "parser/GherkinParser.hpp"

GherkinParser::GherkinParser(const std::string& filename)
    : m_fileReader(nullptr)
    , m_tokenScanner(nullptr)
    , m_tokenMatcher(nullptr)
    , m_bulder(nullptr)
    , m_parser(nullptr)
{
    m_fileReader = FileReader_new(filename.c_str());

    // Gherkin-C's strange method of handling file not found. Handle it.
    // https://github.com/cucumber/gherkin-c/commit/db6ae6d2b99e68df47d774aac35f35109dc807a1
    const wchar_t* fileContents = FileReader_read(m_fileReader);
    if(fileContents == L" ")
    {
        throw FileNotFoundException(filename);
    }

    m_tokenScanner = StringTokenScanner_new(fileContents);
    m_tokenMatcher = TokenMatcher_new(L"en.US");
    m_bulder = AstBuilder_new();
    m_parser = Parser_new(m_bulder);
}

GherkinParser::~GherkinParser()
{
    FileReader_delete(m_fileReader);
    TokenScanner_delete(m_tokenScanner);
    TokenMatcher_delete(m_tokenMatcher);
    Parser_delete(m_parser);    
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
    Parser_parse(m_parser, m_tokenMatcher, m_tokenScanner);

    std::string errors = ReadParserErrors(m_parser);
    if(!errors.empty())
    {
        throw ParserErrorException(errors);
    }

    return GherkinDocumentPtr(AstBuilder_get_result(m_bulder));
}