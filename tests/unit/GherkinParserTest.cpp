#include <gtest/gtest.h>

#include <cucumber-cpp/internal/connectors/gherkin/parser/GherkinParser.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Exceptions.hpp>
#include "../../3rdparty/gherkin-c/include/gherkin_document.h"
using namespace cucumber::internal;

TEST(GherkinParserTest, missingFileThrowsException)
{
    EXPECT_THROW(GherkinParser::loadFeatureFile("missing file"), FileNotFoundException);
}

TEST(GherkinParserTest, creationWithEmptyStringDoesntThrowException)
{
    EXPECT_NO_THROW(GherkinParser parser(L""));
}

TEST(GherkinParserTest, parsingEmptyStringDoesntThrowException)
{
    GherkinParser parser(L"");
    EXPECT_NO_THROW(parser.parse());
}

TEST(GherkinParserTest, parsingMultipleTimesDoesntThrowException)
{
    GherkinParser parser(L"");
    parser.parse();
    EXPECT_NO_THROW(parser.parse());
}

TEST(GherkinParserTest, parsingBadDataResultsInCorrectParserError)
{
    GherkinParser parser(L"this is not a feature file");
    try {
        parser.parse();
        FAIL() << "Expected ParserErrorException";
    } catch(ParserErrorException& ex) {
        EXPECT_STREQ("Parser errors : (1:1): expected: #EOF, #Language, "
            "#TagLine, #FeatureLine, #Comment, #Empty, got "
            "'this is not a feature file' at 1:1\n", ex.what());
    } catch(...) {
       FAIL() << "Expected ParserErrorException";
    }
}

TEST(GherkinParserTest, parsingValidFileReturnsValidPointer)
{
    const std::wstring source = 
    L"Feature: Test Feature\n"
        L"\tScenario: minimalistic\n"
        L"\t\tGiven the minimalism\n";
    GherkinParser parser(source);
    GherkinDocumentPtr document = parser.parse();
    EXPECT_NE(document.get(), (void*)NULL);
}

TEST(GherkinParserTest, returnedDocumentIsUsableAfterParserDestruction)
{
    GherkinDocumentPtr document;

    {
        const std::wstring source = 
        L"Feature: Test Feature\n"
            L"\tScenario: minimalistic\n"
            L"\t\tGiven the minimalism\n";
        GherkinParser parser(source);
        document = parser.parse();
    }

    EXPECT_EQ(1, document->feature->location.column);
}