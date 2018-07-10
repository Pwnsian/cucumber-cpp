#ifndef GHERKIN_PROTOCOL_CONNECTOR_H
#define GHERKIN_PROTOCOL_CONNECTOR_H

#include <vector>
#include <cucumber-cpp/internal/CukeEngineImpl.hpp>
#include "utility/Types.hpp"
#include "output/IGherkinTestEventListener.hpp"

// Gherkin-C forward declarations
struct Scenario;
struct ScenarioOutline;
struct Background;
struct Tags;
struct TableRow;

namespace cucumber {
namespace internal {

class GherkinProtocolUtils
{
public:
    typedef std::map<std::string, std::vector<std::string> > examples_map;

public:
    static std::vector<std::string> getTagsToStringArray(const Tags* tags);
    static std::vector<std::string> getStringArguments(const Step* step);
    static examples_map getExamples(const ::ScenarioOutline* scenarioOutline);
    static std::vector<std::string> getTableRowValues(const TableRow* tableRow);
    static std::string replaceStepTextWithExample(const std::string& step_text, const examples_map& examples, int exampleNumber);
    static void validateScenarioOutlineExamples(const examples_map& examples);
};

class GherkinProtocolConnector
{
public:
    GherkinProtocolConnector(CukeEngine* engine);
    void acceptOnce(GherkinDocumentPtr document);
    void setListener(IGherkinTestEventListener* listener);

private:
    void runScenario(const ::Scenario* scenario);
    void runScenarioOutline(const ::ScenarioOutline* scenarioOutline);
    void runBackground(const Background* background);
    void runExamples(const Steps* steps, const GherkinProtocolUtils::examples_map& examples);
    void runSteps(const Steps* steps);
    void runStep(const Step* step);
    void runStep(const Step* step, const std::string& fullStepText);

private:
    CukeEngine* m_engine;
    boost::movelib::unique_ptr<IGherkinTestEventListener> m_listener;
};

}
}

#endif
