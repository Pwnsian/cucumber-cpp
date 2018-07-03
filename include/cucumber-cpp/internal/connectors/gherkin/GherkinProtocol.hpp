#ifndef GHERKIN_ENGINE_H
#define GHERKIN_ENGINE_H

#include <vector>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Types.hpp>
#include <cucumber-cpp/internal/CukeEngineImpl.hpp>

// Gherkin-C forward declarations
class Scenario;
class ScenarioOutline;
class Tags;

namespace cucumber {
namespace internal {

class GherkinProtocolConnector
{
public:
    GherkinProtocolConnector(GherkinDocumentPtr document);
    void acceptOnce();

private:
    void runScenario(const ::Scenario* scenario);
    void runScenarioOutline(const ::ScenarioOutline* scenarioOutline);
    std::vector<std::string> readRagsToStringArray(const Tags* tags);

private:
    GherkinDocumentPtr m_document;
    CukeEngineImpl m_engine;
};

}
}

#endif
