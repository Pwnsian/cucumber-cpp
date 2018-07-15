#include <cucumber-cpp/internal/connectors/gherkin/output/GherkinProductionPrinter.hpp>
#include <cucumber-cpp/internal/connectors/gherkin/utility/Utility.hpp>
#include <iostream>

#include <boost/test/unit_test.hpp>

using namespace cucumber::internal;

GherkinProductionPrinter::GherkinProductionPrinter(std::ostream& out)
    : m_output(out)
{

}

bool GherkinProductionPrinter::hasTestFailures() const
{
    return !m_scenarioFailureCounts.empty();
}

const GherkinProductionPrinter::failure_report& GherkinProductionPrinter::getTestFailures() const
{
    return m_scenarioFailureCounts;
}

void GherkinProductionPrinter::onFeatureStarted(const std::string& featureName)
{
    m_output << "[  FEATURE  ] " << featureName << std::endl;
}

void GherkinProductionPrinter::onFeatureEnd(const std::string& featureName)
{
    typedef failure_report::const_iterator const_iterator;
    const failure_report& failures = m_scenarioFailureCounts;

    unsigned totalFailureCount = 0;
    for (const_iterator it = failures.begin(); it != failures.end(); ++it)
    {
        totalFailureCount += it->second;
    }

    m_output << "[  FEATURE  ] " << featureName
             << " contained " << totalFailureCount << " failure(s)" << std::endl;

    for (const_iterator it = failures.begin(); it != failures.end(); ++it)
    {
        m_output << "[           ] [" << it->first << "]: " << it->second << std::endl;
    }

    m_output << std::endl;
}

void GherkinProductionPrinter::onScenarioStarted(const std::string& scenarioName)
{
    m_output << "[ SCENARIO  ] " << scenarioName << std::endl;
    m_currentScenario = scenarioName;   
}

void GherkinProductionPrinter::onScenarioEnd(const std::string& scenarioName)
{
    unsigned scenarioFailureCount = 0;
    if (m_scenarioFailureCounts.count(scenarioName))
        scenarioFailureCount = m_scenarioFailureCounts[scenarioName];

    m_output << "[ SCENARIO  ] [" << scenarioName
             << "] contained " << scenarioFailureCount
             << " failed test(s)." << std::endl << std::endl;
}

void GherkinProductionPrinter::onTestStarted(const std::string& testName)
{
    m_output << "[ RUN       ] [" << m_currentScenario << "] " << testName << std::endl;
}

void GherkinProductionPrinter::onTestPassed(const std::string& testName)
{
    m_output << "[        OK ] [" << m_currentScenario << "] " << testName << std::endl;
}

void GherkinProductionPrinter::onTestPending(const std::string& testName, const std::string& pendingReason)
{
    m_output << "[      PEND ] [" << m_currentScenario << "] " << testName << std::endl;
    m_output << pendingReason << std::endl;
    m_scenarioFailureCounts[m_currentScenario] += 1;
}

void GherkinProductionPrinter::onTestFailed(const std::string& testName, const std::string& failureReason, const std::string& failureType)
{
    m_output << "[      FAIL ] [" << m_currentScenario << "] " << testName << std::endl;
    m_output << failureReason << std::endl;    
    m_output << failureType << std::endl;
    m_scenarioFailureCounts[m_currentScenario] += 1;
}
