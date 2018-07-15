#ifndef GHERKIN_PROTOCOL_PRINTER_H
#define GHERKIN_PROTOCOL_PRINTER_H
#include <iosfwd>
#include <map>
#include <boost/move/unique_ptr.hpp>
#include <boost/chrono.hpp>
#include "IGherkinTestEventListener.hpp"

namespace Catch
{
    struct ReporterConfig;
    class JunitReporter;
}

class GherkinJunitReporter : public GherkinEmptyTestEventListener
{
public:
    GherkinJunitReporter(std::ostream& outStream);

public:
    virtual void onFeatureStarted(const std::string&);
    virtual void onFeatureEnd(const std::string&);
    virtual void onScenarioStarted(const std::string&);
    virtual void onScenarioEnd(const std::string&);
    virtual void onTestStarted(const std::string&);
    virtual void onTestPassed(const std::string&);
    virtual void onTestPending(const std::string&, const std::string&);
    virtual void onTestFailed(const std::string&, const std::string&, const std::string&);

private:
    boost::movelib::unique_ptr<Catch::ReporterConfig> m_reporterConfig;
    boost::movelib::unique_ptr<Catch::JunitReporter> m_reporter;
    boost::chrono::steady_clock::time_point m_currentTestStartTime;
    std::ostream& m_output;
};

#endif