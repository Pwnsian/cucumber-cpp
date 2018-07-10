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
#include <iostream>

namespace cucumber {
namespace internal {

GherkinProtocolConnector::GherkinProtocolConnector(CukeEngine* engine)
    : m_engine(engine)
    , m_listener(new GherkinEmptyTestEventListener())
{
    
}

void GherkinProtocolConnector::setListener(IGherkinTestEventListener* listener)
{
    m_listener.reset(listener);
}

void GherkinProtocolConnector::acceptOnce(GherkinDocumentPtr document)
{
    const Feature* documentFeature = document->feature;
    const ScenarioDefinitions* scenarios = documentFeature->scenario_definitions;

    m_listener->onFeatureStarted(narrowString(documentFeature->name));

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

    m_listener->onFeatureEnd(narrowString(documentFeature->name));
}

void GherkinProtocolConnector::runScenario(const ::Scenario* scenario)
{
    std::vector<std::string> tags = GherkinProtocolUtils::getTagsToStringArray(scenario->tags);
    std::string narrowScenarioName = narrowString(scenario->name);

    m_engine->beginScenario(tags);
    m_listener->onScenarioStarted(narrowScenarioName);

    try
    {
        runSteps(scenario->steps);
    }
    catch(std::exception&)
    {
        m_listener->onScenarioEnd(narrowScenarioName);
        m_engine->endScenario(tags);
        throw;
    }

    m_listener->onScenarioEnd(narrowScenarioName);
    m_engine->endScenario(tags);
}

void GherkinProtocolConnector::runScenarioOutline(const ScenarioOutline* scenarioOutline)
{
    std::vector<std::string> tags = GherkinProtocolUtils::getTagsToStringArray(scenarioOutline->tags);
    std::string narrowScenarioName = narrowString(scenarioOutline->name);

    m_listener->onScenarioStarted(narrowScenarioName);
    m_engine->beginScenario(tags);
    
    try
    {
        runExamples(scenarioOutline->steps, GherkinProtocolUtils::getExamples(scenarioOutline));
    }
    catch(std::exception&)
    {
        m_listener->onScenarioEnd(narrowScenarioName);
        m_engine->endScenario(tags);
        throw;
    }

    m_listener->onScenarioEnd(narrowScenarioName);
    m_engine->endScenario(tags);
}

void GherkinProtocolConnector::runBackground(const Background* background)
{
    runSteps(background->steps);
}

void GherkinProtocolConnector::runExamples(
    const Steps* steps,
    const GherkinProtocolUtils::examples_map& examples)
{
    GherkinProtocolUtils::validateScenarioOutlineExamples(examples);

    size_t numberOfExamples = examples.begin()->second.size();
    for(size_t example = 0; example < numberOfExamples; ++example)
    {
        for(int step = 0; step < steps->step_count; ++ step)
        {
            const Step* thisStep = &steps->steps[step];
            std::string initialStepText = narrowString(thisStep->text);
            std::string finalStepText = GherkinProtocolUtils::replaceStepTextWithExample(initialStepText, examples, example);
            runStep(thisStep, finalStepText);
        }
    }
}

void GherkinProtocolConnector::runSteps(const Steps* steps)
{
    for(int i = 0; i < steps->step_count; ++i)
    {
        runStep(&steps->steps[i]);
    }
}

void GherkinProtocolConnector::runStep(const Step* step)
{
    std::string narrowStepText = narrowString(step->text);
    runStep(step, narrowStepText);
}

void GherkinProtocolConnector::runStep(const Step* step, const std::string& fullStepText)
{
    std::vector<StepMatch> stepMatches = m_engine->stepMatches(fullStepText);

    if(stepMatches.empty())
    {
        std::string stepSnippet = m_engine->snippetText(narrowString(step->keyword), fullStepText, "");
        throw MissingStepDefinitionException(fullStepText, stepSnippet);
    }
    else if(stepMatches.size() > 1)
    {
        throw AmbiguousStepDefinitionException(fullStepText, stepMatches);
    }
    else
    {
        const StepMatch& matchedStep = *stepMatches.begin();
        CukeEngine::invoke_table_type emptyTableArgs;
        CukeEngine::invoke_args_type arguments;
        for (std::vector<StepMatchArg>::const_iterator it = matchedStep.args.begin(); it != matchedStep.args.end(); ++it)
        {
            arguments.push_back(it->value);
        }

        try {
            m_listener->onTestStarted(fullStepText);
            m_engine->invokeStep(matchedStep.id, arguments, emptyTableArgs);
            m_listener->onTestPassed(fullStepText);
        } catch (const InvokeFailureException& e) {
            m_listener->onTestFailed(fullStepText, e.getMessage(), e.getExceptionType());
        } catch (const PendingStepException& e) {
            m_listener->onTestPending(fullStepText, e.getMessage());
        } catch (...) {
            m_listener->onTestFailed(fullStepText, "Unknown Failure", "Unknown");
        }
    }
}

std::vector<std::string> GherkinProtocolUtils::getTagsToStringArray(const Tags* tags)
{
    std::vector<std::string> result;
    for(int i = 0; i < tags->tag_count; ++i)
    {
        result.push_back(narrowString(tags->tags[i].name));
    }
    return result;
}

std::vector<std::string> GherkinProtocolUtils::getStringArguments(const Step* step)
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

GherkinProtocolUtils::examples_map GherkinProtocolUtils::getExamples(
	const ::ScenarioOutline* scenarioOutline)
{
    GherkinProtocolUtils::examples_map result;

    for(int i = 0; i < scenarioOutline->examples->example_count; ++i)
    {
        const ExampleTable* examples = &scenarioOutline->examples->example_table[i];
        std::vector<std::string> headers = getTableRowValues(examples->table_header);

        const TableRows* tableBody = examples->table_body;
        for(int j = 0; j < tableBody->row_count; ++j)
        {
            const TableRow* bodyRow = &tableBody->table_rows[j];
            std::vector<std::string> examplesCells = getTableRowValues(bodyRow);

            for(unsigned k = 0; k < examplesCells.size(); ++k)
            {
                result[headers[k]].push_back(examplesCells[k]);
            }
        }
    }

    return result;
}

std::vector<std::string> GherkinProtocolUtils::getTableRowValues(const TableRow* tableRow)
{
    std::vector<std::string> result;

    for(int i = 0; i < tableRow->table_cells->cell_count; ++i)
    {
        const TableCell* cell = &tableRow->table_cells->table_cells[i];
        if(cell->value)
        {
            result.push_back(narrowString(cell->value));
        }
    }

    return result;
}


std::string GherkinProtocolUtils::replaceStepTextWithExample(
    const std::string& step_text,
    const GherkinProtocolUtils::examples_map& examples,
    int exampleNumber)
{
    typedef GherkinProtocolUtils::examples_map::const_iterator const_iterator;

    std::string exampleStepText = step_text;

    for(const_iterator it = examples.begin(); it != examples.end(); ++it)
    {
        std::string searchText = "<" + it->first + ">";
        std::string replacementText = it->second[exampleNumber];
        replaceAll(exampleStepText, searchText, replacementText);
    }

    return exampleStepText;
}


void GherkinProtocolUtils::validateScenarioOutlineExamples(const examples_map& examples)
{
    if(examples.empty())
    {
        throw StringException("Scenario Outline missing examples in feature");
    }
}

}
}