#include <cucumber-cpp/internal/connectors/gherkin/GherkinProtocol.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/parser/GherkinParser.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Exceptions.hpp>

#include <gtest/gtest.h>
#include <boost/assign/list_of.hpp>
#include "../utils/MockCukeEngine.hpp"

using namespace cucumber::internal;
using namespace testing;

class GherkinProtocolTest : public Test
{
protected:
    virtual void SetUp()
    {
        minimal_feature = parseDocument("../../3rdparty/gherkin-c/testdata/good/minimal.feature");
        tags_feature = parseDocument("../../3rdparty/gherkin-c/testdata/good/tags.feature");
    }

    GherkinDocumentPtr parseDocument(const std::string& filename)
    {
        GherkinParser parser(GherkinParser::loadFeatureFile(filename));
        return parser.parse();
    }

protected:
    GherkinDocumentPtr minimal_feature;
    GherkinDocumentPtr tags_feature;
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

    connector.acceptOnce(tags_feature);
}

// Test outline with no examples
// Make sure examples with tags are reported to engine