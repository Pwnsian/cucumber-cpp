#ifndef IGHERKIN_LISTENER_H
#define IGHERKIN_LISTENER_H

#include <string>

class IGherkinTestEventListener
{
public:
    virtual ~IGherkinTestEventListener(){};

    virtual void onFeatureStarted(const std::string& featureName) = 0;
    virtual void onFeatureEnd(const std::string& featureName) = 0;
    virtual void onScenarioStarted(const std::string& scenarioName) = 0;
    virtual void onScenarioEnd(const std::string& scenarioName) = 0;
    virtual void onTestStarted(const std::string& testName) = 0;
    virtual void onTestPassed(const std::string& testName) = 0;
    virtual void onTestPending(const std::string& testName, const std::string& pendingReason) = 0;
    virtual void onTestFailed(const std::string& testName, const std::string& failureReason, const std::string& failureType) = 0;
};

class GherkinEmptyTestEventListener : public IGherkinTestEventListener
{
public:
    virtual void onFeatureStarted(const std::string&) { }
    virtual void onFeatureEnd(const std::string&) { }
    virtual void onScenarioStarted(const std::string&) { }
    virtual void onScenarioEnd(const std::string&) { }
    virtual void onTestStarted(const std::string&) { }
    virtual void onTestPassed(const std::string&) { }
    virtual void onTestPending(const std::string&, const std::string&) { }
    virtual void onTestFailed(const std::string&, const std::string&, const std::string&) { }
};

#endif
