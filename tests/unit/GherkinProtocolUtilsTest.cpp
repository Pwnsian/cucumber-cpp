#include <cucumber-cpp/internal/connectors/gherkin/GherkinProtocol.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/parser/GherkinParser.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Exceptions.hpp>
#include "../../3rdparty/gherkin-c/include/gherkin_document.h"
#include "../../3rdparty/gherkin-c/include/scenario.h"
#include "../../3rdparty/gherkin-c/include/scenario_outline.h"

#include <gtest/gtest.h>
#include <boost/assign/list_of.hpp>
#include "../utils/MockCukeEngine.hpp"

using namespace cucumber::internal;
using namespace testing;

class GherkinProtocolUtilsTest : public Test
{
protected:
    virtual void SetUp()
    {
        tags_feature = parseDocument("../../3rdparty/gherkin-c/testdata/good/scenario_outlines_with_tags.feature");
        scenario_outlines = parseDocument("../../3rdparty/gherkin-c/testdata/good/scenario_outline_with_docstring.feature");
    }

    GherkinDocumentPtr parseDocument(const std::string& filename)
    {
        GherkinParser parser(GherkinParser::loadFeatureFile(filename));
        return parser.parse();
    }

    const ::ScenarioOutline* getNthScenarioOutlineInFeature(const Feature* feature, int n)
    {
        const ScenarioDefinitions* scenarios = feature->scenario_definitions;
        const ScenarioDefinition* scenario_def = scenarios->scenario_definitions[n];

        if(scenario_def->type != Gherkin_ScenarioOutline)
        {
            throw StringException("Requested scenario was not a Scenario outline");
        }

        return (const ::ScenarioOutline*)scenario_def;
    }

protected:
    GherkinDocumentPtr scenario_outlines;
    GherkinDocumentPtr tags_feature;
};

TEST_F(GherkinProtocolUtilsTest, getTagsToStringArrayReturnsCorrectFeatureTags)
{
    std::vector<std::string> actualTags = GherkinProtocolUtils::getTagsToStringArray(tags_feature->feature->tags);
    EXPECT_EQ(boost::assign::list_of("@a"), actualTags);
}

TEST_F(GherkinProtocolUtilsTest,  getTagsToStringArrayReturnsCorrectScenarioOutlineTags)
{
    const ::ScenarioOutline* outline = getNthScenarioOutlineInFeature(tags_feature->feature, 0);
    std::vector<std::string> actualTags = GherkinProtocolUtils::getTagsToStringArray(outline->tags);
    EXPECT_EQ(boost::assign::list_of("@b")("@c"), actualTags);
}

TEST_F(GherkinProtocolUtilsTest, getTagsToStringArrayReturnsEmptyListWithNoTags)
{
    std::vector<std::string> actualTags = GherkinProtocolUtils::getTagsToStringArray(scenario_outlines->feature->tags);
    EXPECT_TRUE(actualTags.empty());
}

TEST_F(GherkinProtocolUtilsTest, getStringArgumentsRetrunsDocStringArguments)
{
    // Note: The DocString on this Example is returned incorrectly from Gherkin-C.
    // The incorrect value is tested here. (The string should begin with "<type>).

    const ::ScenarioOutline* outline = getNthScenarioOutlineInFeature(scenario_outlines->feature, 0);
    const Step* step = &outline->steps->steps[0];
    std::vector<std::string> actualArguments = GherkinProtocolUtils::getStringArguments(step);
    std::vector<std::string> expectedArguments = boost::assign::list_of("Greeting:<content>");
    EXPECT_EQ(expectedArguments, actualArguments);
}

TEST_F(GherkinProtocolUtilsTest, getStringArgumentsReturnsEmptyListWithNoArguments)
{
    const ::ScenarioOutline* scenario = getNthScenarioOutlineInFeature(tags_feature->feature, 0);
    const Step* step = &scenario->steps->steps[0];
    std::vector<std::string> actualArguments = GherkinProtocolUtils::getStringArguments(step);
    EXPECT_TRUE(actualArguments.empty());
}

TEST_F(GherkinProtocolUtilsTest, getExamplesReturnsCorrectStructure)
{
    const ::ScenarioOutline* outline = getNthScenarioOutlineInFeature(scenario_outlines->feature, 0);
    GherkinProtocolUtils::examples_container examples = GherkinProtocolUtils::getExamples(outline);

    EXPECT_EQ(2, examples.size());
    EXPECT_EQ(boost::assign::list_of("en")("fr"), examples.at("type"));
    EXPECT_EQ(boost::assign::list_of("Hello")("Bonjour"), examples.at("content"));
}