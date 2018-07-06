#ifndef GHERKIN_ENGINE_H
#define GHERKIN_ENGINE_H

#include <vector>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Types.hpp>
#include <cucumber-cpp/internal/CukeEngineImpl.hpp>

// Gherkin-C forward declarations
class Scenario;
class ScenarioOutline;
class Background;
class Tags;
class TableRow;

namespace cucumber {
namespace internal {

class GherkinProtocolConnector
{
public:
    GherkinProtocolConnector(CukeEngine* engine);
    void acceptOnce(GherkinDocumentPtr document);

private:
    void runScenario(const ::Scenario* scenario);
    void runScenarioOutline(const ::ScenarioOutline* scenarioOutline);
    void runBackground(const Background* background);
    void runSteps(const Steps* steps);

private:
    CukeEngine* m_engine;
};

class GherkinProtocolUtils
{
public:
    typedef std::map<std::string, std::vector<std::string> > examples_container;

public:
    static std::vector<std::string> getTagsToStringArray(const Tags* tags);
    static std::vector<std::string> getStringArguments(const Step* step);
    static examples_container getExamples(const ::ScenarioOutline* scenarioOutline);
    static std::vector<std::string> getTableRowValues(const TableRow* tableRow);
    static void validateScenarioOutlineExamples(const examples_container& examples);
};

}
}

#endif
