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
    std::vector<std::string> getTagsToStringArray(const Tags* tags);
    std::vector<std::string> getStringArguments(const Step* step);

private:
    CukeEngine* m_engine;
};

}
}

#endif
