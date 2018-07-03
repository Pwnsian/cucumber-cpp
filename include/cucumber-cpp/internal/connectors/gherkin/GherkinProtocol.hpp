#ifndef GHERKIN_ENGINE_H
#define GHERKIN_ENGINE_H

#include <vector>
#include "utility/types.h"

// cucumber-cpp forward declarations
namespace cucumber{
    namespace internal {
        class CukeEngineImpl;
    }
}

// Gherkin-C forward declarations
class Scenario;
class ScenarioOutline;
class Tags;

class GherkinProtocolConnector
{
public:
    GherkinProtocolConnector(GherkinDocumentPtr document);
    void acceptOnce();

private:
    void runScenario(const Scenario* scenario);
    void runScenarioOutline(const ScenarioOutline* scenarioOutline);
    std::vector<std::string> readRagsToStringArray(const Tags* tags);

private:
    GherkinDocumentPtr m_document;
    std::unique_ptr<cucumber::internal::CukeEngineImpl> m_engine;
};

#endif
