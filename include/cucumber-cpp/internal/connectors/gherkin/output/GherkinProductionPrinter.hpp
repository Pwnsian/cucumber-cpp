#ifndef GHERKIN_PROTOCOL_PRINTER_H
#define GHERKIN_PROTOCOL_PRINTER_H
#include <iosfwd>
#include <map>
#include "IGherkinTestEventListener.hpp"

namespace cucumber {
namespace internal {

class GherkinProductionPrinter : public IGherkinTestEventListener
{
public:
    typedef std::map<std::string, unsigned> failure_report;

public:
    GherkinProductionPrinter(std::ostream& out);

    bool hasTestFailures() const;
    const failure_report& getTestFailures() const;

public:
    virtual void onFeatureStarted(const std::string& featureName);
    virtual void onFeatureEnd(const std::string& featureName);
    virtual void onScenarioStarted(const std::string& scenarioName);
    virtual void onScenarioEnd(const std::string& scenarioName);
    virtual void onTestStarted(const std::string& testName);
    virtual void onTestPassed(const std::string& testName);
    virtual void onTestPending(const std::string& testName, const std::string& pendingReason);
    virtual void onTestFailed(const std::string& testName, const std::string& failureReason, const std::string& failureType);

private:
    std::string m_currentScenario;
    failure_report m_scenarioFailureCounts;
    std::ostream& m_output;
};

}
}

#endif