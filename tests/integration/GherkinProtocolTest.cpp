#include <cucumber-cpp/internal/connectors/gherkin/GherkinProtocol.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/parser/GherkinParser.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Exceptions.hpp>

#include <gtest/gtest.h>
#include <boost/assign/list_of.hpp>
#include "../utils/MockCukeEngine.hpp"

using namespace cucumber::internal;
using namespace testing;

namespace
{
	const std::wstring minimal_feature_source =
		L"Feature: Minimal\n\n"
		L"  Scenario: minimalistic\n"
		L"    Given the minimalism";

	const std::wstring tags_feature_source =
		L"@feature_tag1 @feature_tag2\n"
		L"  @feature_tag3\n"
		L"Feature: Minimal Scenario Outline\n\n"
		L"@scenario_tag1 @scenario_tag2\n"
		L"   @scenario_tag3\n\n"
		L"Scenario: minimalistic\n"
		L"    Given the minimalism\n"
		L"@so_tag1  @so_tag2\n"
		L"  @so_tag3\n"
		L"Scenario Outline: minimalistic outline\n"
		L"    Given the <what>\n\n"
		L"Examples:\n"
		L"  | what  |\n"
		L"  | Chihaya |";

	const std::wstring missing_outline_examples_source =
		L"Feature: Minimal Scenario Outline\n\n"
		L"Scenario Outline: minimalistic outline\n"
		L"    Given the <what>";

	const std::wstring scenario_outlines_source =
		L"Feature: Multiple Scenario Outlines\n\n"
        L"@sample_tag_here\n"
		L"Scenario Outline: Greetings come in many forms\n"
		L"    Given I ask for <type> and <content>\n"
		L"Examples:\n"
		L"| type | content |\n"
		L"| en | Hello |\n"
		L"| fr | Bonjour|\n"
        L"@sample_tag_here\n"
		L"Scenario Outline: More Greeting Examples\n"
		L"    Given I still ask for <type> and <content>\n"
		L"Examples:\n"
		L"| type | content |\n"
		L"| jp | Konnichiwa |\n"
		L"| es | Hola |";
}

class GherkinProtocolTest : public Test
{
protected:
    virtual void SetUp()
    {
        minimal_feature = parseDocument(minimal_feature_source);
        tags_feature = parseDocument(tags_feature_source);
        missing_outline_examples = parseDocument(missing_outline_examples_source);
        scenario_outlines = parseDocument(scenario_outlines_source);
    }

    GherkinDocumentPtr parseDocument(const std::wstring& contents)
    {
        GherkinParser parser(contents);
        return parser.parse();
    }

protected:
    GherkinDocumentPtr minimal_feature;
    GherkinDocumentPtr tags_feature;
    GherkinDocumentPtr missing_outline_examples;
    GherkinDocumentPtr scenario_outlines;
};

TEST_F(GherkinProtocolTest, runningMinimalFeatureMakesAllCorrectEngineCalls)
{
    StrictMock<MockCukeEngine> engine;
    GherkinProtocolConnector connector(&engine);

    EXPECT_CALL(engine, beginScenario(CukeEngine::tags_type()))
        .Times(1);

    EXPECT_CALL(engine, stepMatches("the minimalism"))
        .Times(1)
        .WillRepeatedly(Return(std::vector<StepMatch>(1)));

    EXPECT_CALL(engine, invokeStep("", CukeEngine::invoke_args_type(), CukeEngine::invoke_table_type()))
        .Times(1);              

    EXPECT_CALL(engine, endScenario(CukeEngine::tags_type()))
        .Times(1); 

    connector.acceptOnce(minimal_feature);
}

TEST_F(GherkinProtocolTest, noStepMatchesThrowsMissingStepException)
{
    NiceMock<MockCukeEngine> engine;
    GherkinProtocolConnector connector(&engine);

    ON_CALL(engine, stepMatches(_))
        .WillByDefault(Return(std::vector<StepMatch>(0)));

    EXPECT_THROW(connector.acceptOnce(minimal_feature), MissingStepDefinitionException);
}

TEST_F(GherkinProtocolTest, multipleStepMatchesThrowsAmbiguousStepException)
{
    NiceMock<MockCukeEngine> engine;
    GherkinProtocolConnector connector(&engine);

    ON_CALL(engine, stepMatches(_))
        .WillByDefault(Return(std::vector<StepMatch>(2)));

    EXPECT_THROW(connector.acceptOnce(minimal_feature), AmbiguousStepDefinitionException);
}

TEST_F(GherkinProtocolTest, throwingExceptionsBeginsAndEndsScenarioCorrectly)
{
    NiceMock<MockCukeEngine> engine;
    GherkinProtocolConnector connector(&engine);
    
    EXPECT_CALL(engine, beginScenario(CukeEngine::tags_type()))
        .Times(1);

    ON_CALL(engine, stepMatches(_))
        .WillByDefault(Return(std::vector<StepMatch>(0)));     

    EXPECT_CALL(engine, endScenario(CukeEngine::tags_type()))
        .Times(1); 

    EXPECT_THROW(connector.acceptOnce(minimal_feature), MissingStepDefinitionException);
}

TEST_F(GherkinProtocolTest, beginsAndEndsScenarioWithTagsCorrectly)
{
    NiceMock<MockCukeEngine> engine;
    GherkinProtocolConnector connector(&engine);

    CukeEngine::tags_type firstScenarioTags = boost::assign::list_of("@scenario_tag1")("@scenario_tag2")("@scenario_tag3");
    EXPECT_CALL(engine, beginScenario(firstScenarioTags)).Times(1);
    EXPECT_CALL(engine, endScenario(firstScenarioTags)).Times(1); 

    CukeEngine::tags_type secondScenarioTags = boost::assign::list_of("@so_tag1")("@so_tag2")("@so_tag3");
    EXPECT_CALL(engine, beginScenario(secondScenarioTags)).Times(1);
    EXPECT_CALL(engine, endScenario(secondScenarioTags)).Times(1); 

    ON_CALL(engine, stepMatches(_))
        .WillByDefault(Return(std::vector<StepMatch>(1)));

    connector.acceptOnce(tags_feature);
}

TEST_F(GherkinProtocolTest, missingExamplesThrowsException)
{
    NiceMock<MockCukeEngine> engine;
    GherkinProtocolConnector connector(&engine);

    ON_CALL(engine, stepMatches(_))
        .WillByDefault(Return(std::vector<StepMatch>(1)));

    EXPECT_THROW(connector.acceptOnce(missing_outline_examples), StringException);
}

TEST_F(GherkinProtocolTest, invokesStepWithoutArgumentsInvokesCorrectly)
{
    NiceMock<MockCukeEngine> engine;
    GherkinProtocolConnector connector(&engine);

    StepMatch dummyStepMatch;
    dummyStepMatch.id = "test";

    ON_CALL(engine, stepMatches(_))
        .WillByDefault(Return(boost::assign::list_of(dummyStepMatch)));

    EXPECT_CALL(engine, invokeStep("test", CukeEngine::invoke_args_type(), CukeEngine::invoke_table_type()))
        .Times(1);

    connector.acceptOnce(minimal_feature);
}

TEST_F(GherkinProtocolTest, runningScenarioOutlinesMakesAllCorrectEngineCalls)
{
    StrictMock<MockCukeEngine> engine;
    GherkinProtocolConnector connector(&engine);

    StepMatch dummyStepMatch;
    dummyStepMatch.id = "test";

    ON_CALL(engine, stepMatches(_))
        .WillByDefault(Return(boost::assign::list_of(dummyStepMatch)));

    std::vector<std::string> scenarioTags = boost::assign::list_of("@sample_tag_here");
    EXPECT_CALL(engine, beginScenario(scenarioTags)).Times(2);
    EXPECT_CALL(engine, stepMatches("I ask for en and Hello")).Times(1);
    EXPECT_CALL(engine, stepMatches("I ask for fr and Bonjour")).Times(1);
    EXPECT_CALL(engine, stepMatches("I still ask for jp and Konnichiwa")).Times(1);
    EXPECT_CALL(engine, stepMatches("I still ask for es and Hola")).Times(1);
    EXPECT_CALL(engine, invokeStep("test", _, _)).Times(4);
    EXPECT_CALL(engine, endScenario(scenarioTags)).Times(2);

    connector.acceptOnce(scenario_outlines);
}


// Make sure examples with tags are reported to engine