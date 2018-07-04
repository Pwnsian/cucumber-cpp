#include <cucumber-cpp/internal/connectors/gherkin/parser/GherkinParser.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/GherkinProtocol.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Utility.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Exceptions.hpp>
#include <cucumber-cpp/internal/CukeEngineImpl.hpp>
#include <gherkin-c/include/gherkin_document.h>
#include <gherkin-c/include/feature.h>
#include <gherkin-c/include/scenario.h>
#include <gherkin-c/include/scenario_outline.h>
#include <gherkin-c/include/pickle_string.h>
#include <gherkin-c/include/data_table.h>
#include <gherkin-c/include/doc_string.h>

namespace cucumber {
namespace internal {

GherkinProtocolConnector::GherkinProtocolConnector(CukeEngine* engine)
    : m_engine(engine)
{
    
}

void GherkinProtocolConnector::acceptOnce(GherkinDocumentPtr document)
{
    const Feature* documentFeature = document->feature;
    const ScenarioDefinitions* scenarios = documentFeature->scenario_definitions;

    for(int i = 0; i < scenarios->scenario_definition_count; ++i)
    {
        const ScenarioDefinition* scenario_def_i = scenarios->scenario_definitions[i];

        if(scenario_def_i->type == Gherkin_Background)
        {
            const Background* background = (const Background*)scenario_def_i;
            runBackground(background);
        }
        else if(scenario_def_i->type == Gherkin_Scenario)
        {
            const ::Scenario* scenario = (const ::Scenario*)scenario_def_i;
            runScenario(scenario);
        }
        else if(scenario_def_i->type == Gherkin_ScenarioOutline)
        {
            const ScenarioOutline* scenario_outline = (const ScenarioOutline*)scenario_def_i;
            runScenarioOutline(scenario_outline);
        }
    }
}

void GherkinProtocolConnector::runScenario(const ::Scenario* scenario)
{
    std::vector<std::string> tags = getTagsToStringArray(scenario->tags);
    m_engine->beginScenario(tags);
    try
    {

    }
    catch(std::exception& ex)
    {
        m_engine->endScenario(tags);
        throw ex;
    }
    m_engine->endScenario(tags);
}

void GherkinProtocolConnector::runScenarioOutline(const ScenarioOutline* scenarioOutline)
{
    std::vector<std::string> tags = getTagsToStringArray(scenarioOutline->tags);
    m_engine->beginScenario(tags);
    try
    {

    }
    catch(std::exception& ex)
    {
        m_engine->endScenario(tags);
        throw ex;
    }
    m_engine->endScenario(tags);
}

void GherkinProtocolConnector::runBackground(const Background* background)
{
    runSteps(background->steps);
}

void GherkinProtocolConnector::runSteps(const Steps* steps)
{
    for(int i = 0; i < steps->step_count; ++i)
    {
        Step step = steps->steps[i];
        std::string narrowStepName = narrowString(step.text);
        std::vector<StepMatch> stepMatches = m_engine->stepMatches(narrowStepName);

        if(stepMatches.empty())
        {
            std::string stepSnippet = m_engine->snippetText(
                narrowString(step.keyword), 
                narrowString(step.text),
                ""
            );
            throw MissingStepDefinitionException(narrowString(step.text), stepSnippet);
        }
        else if(stepMatches.size() > 1)
        {
            throw AmbiguousStepDefinitionException(narrowString(step.text), stepMatches);
        }
        else
        {
            const StepMatch& matchedStep = *stepMatches.begin();
            std::vector<std::string> arguments = getStringArguments(&step);
            CukeEngine::invoke_table_type emptyTableArgs;
            m_engine->invokeStep(matchedStep.id, arguments, emptyTableArgs);
        }
    }
}

std::vector<std::string> GherkinProtocolConnector::getTagsToStringArray(const Tags* tags)
{
    std::vector<std::string> result;
    for(int i = 0; i < tags->tag_count; ++i)
    {
        result.push_back(narrowString(tags->tags[i].name));
    }
    return result;
}

std::vector<std::string> getStringArguments(const Step* step)
{
    std::vector<std::string> results;
    const StepArgument* argument = step->argument;
    if(step->argument != NULL)
    {
        if(argument->type == Gherkin_DataTable)
        {
            throw NotImplementedException("Data Table Arguments");
        }
        else if(argument->type == Gherkin_DocString)
        {
            const DocString* string = (const DocString*)argument;
            results.push_back(narrowString(string->content));
        }
    }
    return results;
}

}
}