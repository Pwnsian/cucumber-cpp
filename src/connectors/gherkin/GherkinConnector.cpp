#include "GherkinConnector.hpp"
#include "Utility.h"
#include <gherkin-c/include/gherkin_document.h>
#include <gherkin-c/include/feature.h>
#include <gherkin-c/include/scenario.h>
#include <gherkin-c/include/scenario_outline.h>
#include <cucumber-cpp/internal/CukeEngineImpl.hpp>

GherkinProtocolConnector::GherkinProtocolConnector(GherkinDocumentPtr document)
    : m_document(document)
    , m_engine(new cucumber::internal::CukeEngineImpl())
{
    
}

void GherkinProtocolConnector::acceptOnce()
{
    const Feature* documentFeature = m_document->feature;
    const ScenarioDefinitions* scenarios = documentFeature->scenario_definitions;

    for(int i = 0; i < scenarios->scenario_definition_count; ++i)
    {
        const ScenarioDefinition* scenario_def_i = scenarios->scenario_definitions[i];

        if(scenario_def_i->type == Gherkin_Scenario)
        {
            const Scenario* scenario = (const Scenario*)scenario_def_i;
            runScenario(scenario);
        }
        else if(scenario_def_i->type == Gherkin_ScenarioOutline)
        {
            const ScenarioOutline* scenario_outline = (const ScenarioOutline*)scenario_def_i;
            runScenarioOutline(scenario_outline);
        }
        else
        {

        }
    }
}

void GherkinProtocolConnector::runScenario(const Scenario* scenario)
{
    auto tags = readRagsToStringArray(scenario->tags);
    m_engine->beginScenario(tags);

    m_engine->endScenario(tags);
}

void GherkinProtocolConnector::runScenarioOutline(const ScenarioOutline* scenarioOutline)
{
    auto tags = readRagsToStringArray(scenarioOutline->tags);
    m_engine->beginScenario(tags);

    m_engine->endScenario(tags);
}

std::vector<std::string> GherkinProtocolConnector::readRagsToStringArray(const Tags* tags)
{
    std::vector<std::string> result;
    for(int i = 0; i < tags->tag_count; ++i)
    {
        Tag tag_i = tags->tags[i];
        result.push_back(narrowString(tag_i.name));
    }
    return result;
}