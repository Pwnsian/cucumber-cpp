#include <cucumber-cpp/internal/connectors/gherkin/output/GherkunJunitReporter.hpp>
#include <boost/move/make_unique.hpp>
#include <boost/chrono/chrono.hpp>

// Catch provides the JUnit report generator.
#define CATCH_CONFIG_RUNNER
#include <cucumber-cpp/internal/catch.hpp>

using namespace boost::chrono;

namespace
{
    void empty()
    {

    }
}

GherkinJunitReporter::GherkinJunitReporter(std::ostream& outStream)
    : m_output(outStream)
{
    m_reporterConfig.reset(new Catch::ReporterConfig(std::shared_ptr<Catch::Config>(), m_output));
    m_reporter.reset(new Catch::JunitReporter(*m_reporterConfig));
}

void GherkinJunitReporter::onFeatureStarted(const std::string&)
{

}

void GherkinJunitReporter::onFeatureEnd(const std::string&)
{

}

void GherkinJunitReporter::onScenarioStarted(const std::string& scenarioName)
{
    Catch::GroupInfo info(scenarioName, 0, 11);
    m_reporter->testGroupStarting(info);
}

void GherkinJunitReporter::onScenarioEnd(const std::string& scenarioName)
{
    m_reporter->testGroupEnded()
}

void GherkinJunitReporter::onTestStarted(const std::string&)
{
    m_currentTestStartTime = steady_clock::now();
}

void GherkinJunitReporter::onTestPassed(const std::string& testName)
{
    steady_clock::time_point now = steady_clock::now();
    milliseconds duration = duration_cast<milliseconds>(now - m_currentTestStartTime);
    m_formatter->test_unit_finish(m_output, *makeTestCase(testName), duration.count());
}

void GherkinJunitReporter::onTestPending(const std::string& testName, const std::string& pendingReason)
{
    m_formatter->test_unit_skipped(m_output, *makeTestCase(testName), pendingReason);
}

void GherkinJunitReporter::onTestFailed(const std::string& testName, const std::string&, const std::string&)
{
    m_formatter->test_unit_aborted(m_output, *makeTestCase(testName));
}
