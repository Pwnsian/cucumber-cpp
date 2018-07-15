/*
 * JunitReporter.h
 *
 *  Created on: 1 Kas 2017
 *      Author: Ozen Ozkaya
 */

#ifndef INC_JUNITREPORTER_H_
#define INC_JUNITREPORTER_H_

#include "JUnitStructures.h"

#ifndef JUNITREPORTER_DEFAULT_FILE
#define JUNITREPORTER_DEFAULT_FILE "tsn_junit_report.xml"
#endif

/* If you know not much about JUnit XML format then you should visit:
 * https://www.ibm.com/support/knowledgecenter/en/SSQ2R2_9.5.0/com.ibm.rsar.analysis.codereview.cobol.doc/topics/cac_useresults_junit.html
 *
 * After reading this link, you will be in an enlightenment state.
 * This library is written for generating JUnit formatted XML reports easily.
 * */


class JUnitReporter {
public:
	JUnitReporter();
	JUnitReporter(char* report_filename);
	void Report(JUnitTestSuites_t* testSuitesPtr);
	JUnitTestSuites_t* CreateTestSuiteSet(const char* name, const char* text);
	JUnitTestSuite_t* AddTestSuite(JUnitTestSuites_t* parent, const char* name, const char* text);
	virtual ~JUnitReporter();
private:
	char* xml_report_filename;
	unsigned int num_test_suites;
	void ReportTestSuite(JUnitTestSuite_t* testSuitePtr);
	void ReportTestCase(JUnitTestCase_t* testCasePtr);
	void ReportTestFailure(JUnitTestStatus_t* testStatusPtr);

};

#endif /* INC_JUNITREPORTER_H_ */
