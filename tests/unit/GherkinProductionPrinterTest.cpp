#include <gtest/gtest.h>
#include <boost/move/unique_ptr.hpp>

#include <cucumber-cpp/internal/connectors/gherkin/output/GherkinProductionPrinter.hpp>
using namespace cucumber::internal;

class GherkinProductionPrinterTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        printer.reset(new GherkinProductionPrinter(output));
    }

    boost::movelib::unique_ptr<GherkinProductionPrinter> printer;
    std::stringstream output;
};

TEST_F(GherkinProductionPrinterTest, beginningFeaturePrintsExpected)
{
    printer->onFeatureStarted("test");
    EXPECT_EQ("[  FEATURE  ] test\n", output.str());
}

TEST_F(GherkinProductionPrinterTest, runningTestInScenarioPrintsExpected)
{
    printer->onScenarioStarted("my scenario");
    printer->onTestStarted("myTest");
    EXPECT_EQ("[ SCENARIO  ] my scenario\n[ RUN       ] [my scenario] myTest\n", output.str());
}

TEST_F(GherkinProductionPrinterTest, failingTestReportsErrorsCorrectly)
{
    printer->onTestFailed("test", "failure reason here", "failure type");
    EXPECT_TRUE(printer->hasTestFailures());
}

TEST_F(GherkinProductionPrinterTest, pendingTestReportsErrorsCorrectly)
{
    printer->onTestPending("test", "my pending reason");
    EXPECT_TRUE(printer->hasTestFailures());
}

TEST_F(GherkinProductionPrinterTest, failingTestsReportFailuesByScenario)
{
    printer->onScenarioStarted("scenario 0");
    printer->onTestFailed("test 0", "failure reason here", "failure type");
    printer->onScenarioStarted("scenario 1");
    printer->onTestFailed("test 1", "another failure reason here", "failure type");
    printer->onTestFailed("test 2", "another failure reason here", "failure type");

    GherkinProductionPrinter::failure_report failures = printer->getTestFailures();

    EXPECT_EQ(2, failures.size());
    EXPECT_EQ(1, failures.at("scenario 0"));
    EXPECT_EQ(2, failures.at("scenario 1"));
}