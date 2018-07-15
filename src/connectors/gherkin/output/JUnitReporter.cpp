/*
 * JunitReporter.cpp
 *
 *  Created on: 1 Kas 2017
 *      Author: Ozen Ozkaya
 */


#include "JUnitReporter.h"
#include "assert.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

/* If you know not much about JUnit XML format then you should visit:
 * https://www.ibm.com/support/knowledgecenter/en/SSQ2R2_9.5.0/com.ibm.rsar.analysis.codereview.cobol.doc/topics/cac_useresults_junit.html
 *
 * After reading this link, you will be in an enlightenment state.
 * This library is written for generating JUnit formatted XML reports easily.
 * */

JUnitReporter::JUnitReporter() {

	/** Default constructor uses default log file name*/
	xml_report_filename = (char*)JUNITREPORTER_DEFAULT_FILE;
	num_test_suites=0;
	std::ofstream junit_report_file;
	/** Open file with overwrite mode*/
	junit_report_file.open (xml_report_filename);
	/** Print standard xml header */
	junit_report_file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
	junit_report_file.close();

}

JUnitReporter::JUnitReporter(char* report_filename)
{
	assert(report_filename);
	num_test_suites=0;
	/** This constructor uses argument log file name*/
	xml_report_filename = (char*)report_filename;
	std::ofstream junit_report_file;
	/** Open file with overwrite mode*/
	junit_report_file.open (xml_report_filename);
	/** Print standard xml header */
	junit_report_file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
	junit_report_file.close();
}

void JUnitReporter::Report(JUnitTestSuites_t* testSuitesPtr)
{
	assert(testSuitesPtr);
	std::ofstream junit_report_file;
	/** Open file with append mode*/
	junit_report_file.open(xml_report_filename, std::ios_base::app);
	/** Append testsuites xml tag start */
	junit_report_file << "<testsuites ";

	/** Append testsuites id, as long as it is greater than zero */
	if( testSuitesPtr->id > 0 )
	{
		junit_report_file << "id=\"" << testSuitesPtr->id <<"\" ";
	}

	/** Append testsuites name, as long as it is a valid string */
	if( testSuitesPtr->name && strlen(testSuitesPtr->name) > 0 )
	{
		junit_report_file << "name=\"" << testSuitesPtr->name <<"\" ";
	}

	/** Append number of tests*/
	junit_report_file << "tests=\"" << testSuitesPtr->num_of_tests <<"\" ";

	/** Append number of failures*/
	junit_report_file << "failures=\"" << testSuitesPtr->num_of_failures <<"\" ";

	/** Append execution time of the testsuites in seconds*/
	junit_report_file << "time=\"" << std::fixed  << testSuitesPtr->time_in_sec <<"\" >" << std::endl;

	/** As ReportTestSuite function will open file, we can close it now.*/
	junit_report_file.close();

	for(unsigned int i=0;i<testSuitesPtr->num_of_test_suites; i++)
	{
		/** Append each testsuite*/
		ReportTestSuite(&testSuitesPtr->test_suite_list[i]);
	}

	/** Reopen file in append mode*/
	junit_report_file.open(xml_report_filename, std::ios_base::app);

	/** Append testsuites xml tag end */
	junit_report_file << "</testsuites>"<< std::endl;
	junit_report_file.close();
}

JUnitTestSuites_t* JUnitReporter::CreateTestSuiteSet(const char* name, const char* text)
{
	JUnitTestSuites_t* JUnitTestSuites_ptr = (JUnitTestSuites_t*)malloc(sizeof(JUnitTestSuites_t));
	num_test_suites++;
	JUnitTestSuites_ptr->id = num_test_suites;
	JUnitTestSuites_ptr->name = name;
	JUnitTestSuites_ptr->text = text;
	return JUnitTestSuites_ptr;
}

JUnitTestSuite_t* JUnitReporter::AddTestSuite(JUnitTestSuites_t* parent, const char* name, const char* text)
{
	JUnitTestSuite_t* JUnitTestSuite_ptr = (JUnitTestSuite_t*)malloc(sizeof(JUnitTestSuite_t));
	parent->num_of_test_suites++;

	return JUnitTestSuite_ptr;

}

void JUnitReporter::ReportTestSuite(JUnitTestSuite_t* testSuitePtr)
{
	assert(testSuitePtr);

	/** Open file with append mode*/
	std::ofstream junit_report_file;
	junit_report_file.open(xml_report_filename, std::ios_base::app);

	/** Append testsuite xml tag start */
	junit_report_file << "\t<testsuite ";

	/** Append testsuite id, as long as it is greater than zero */
	if( testSuitePtr->id > 0 )
	{
		junit_report_file << "id=\"" << testSuitePtr->id <<"\" ";
	}

	/** Append testsuite name, as long as it is a valid string */
	if( testSuitePtr->name && strlen(testSuitePtr->name) > 0 )
	{
		junit_report_file << "name=\"" << testSuitePtr->name <<"\" ";
	}

	/** Append number of tests*/
	junit_report_file << "tests=\"" << testSuitePtr->num_of_tests <<"\" ";
	/** Append number of failures*/
	junit_report_file << "failures=\"" << testSuitePtr->num_of_failures <<"\" ";
	/** Append execution time of the testsuite in seconds*/
	junit_report_file << "time=\"" << std::fixed << testSuitePtr->time_in_sec <<"\" >" << std::endl;

	/** As ReportTestCase function will open file, we can close it now.*/
	junit_report_file.close();

	for(unsigned int i=0;i<testSuitePtr->num_of_test_cases; i++)
	{
		ReportTestCase(&testSuitePtr->test_case_list[i]);
	}

	/** Open file with append mode*/
	junit_report_file.open(xml_report_filename, std::ios_base::app);
	/** Append testsuite xml tag end */
	junit_report_file << "\t</testsuite>" << std::endl;
	junit_report_file.close();

}

void JUnitReporter::ReportTestCase(JUnitTestCase_t* testCasePtr)
{
	assert(testCasePtr);

	/** Open file with append mode*/
	std::ofstream junit_report_file;
	junit_report_file.open(xml_report_filename, std::ios_base::app);

	/** Append testcase xml tag start */
	junit_report_file << "\t\t<testcase ";

	/** Append id, as long as it is greater than zero */
	if( testCasePtr->id > 0 )
	{
		junit_report_file << "id=\"" << testCasePtr->id <<"\" ";
	}

	/** Append name name, as long as it is a valid string */
	if( testCasePtr->name && strlen(testCasePtr->name) > 0 )
	{
		junit_report_file << "name=\"" << testCasePtr->name <<"\" ";
	}

	/** Append execution time in seconds*/
	junit_report_file << "time=\"" << std::fixed << testCasePtr->time_in_sec <<"\" >" << std::endl;

	/** As ReportTestStatus function will open file, we can close it now.*/
	junit_report_file.close();

	for(unsigned int i=0;i<testCasePtr->num_of_test_status; i++)
	{
		ReportTestFailure(&testCasePtr->test_status_list[i]);
	}

	/** Open file with append mode*/
	junit_report_file.open(xml_report_filename, std::ios_base::app);

	/** Append testcase xml tag end */
	junit_report_file << "\t\t</testcase>" << std::endl;
	junit_report_file.close();
}

void JUnitReporter::ReportTestFailure(JUnitTestStatus_t* testStatusPtr)
{
		assert(testStatusPtr);

		/** Open file with append mode*/
		std::ofstream junit_report_file;
		junit_report_file.open(xml_report_filename, std::ios_base::app);

		/** Append failure xml tag start */
		junit_report_file << "\t\t\t<failure ";

		/** Append failure message if it is a valid string */
		if( testStatusPtr->msg != NULL  && strlen(testStatusPtr->msg) > 0 )
		{
			junit_report_file << "message=\"" << testStatusPtr->msg <<"\" ";
		}

		/** Append failure type if it is a valid string */
		if( testStatusPtr->type && strlen(testStatusPtr->type) > 0 )
		{
			junit_report_file << "type=\"" << testStatusPtr->type <<"\" >" << std::endl;
		}

		/** Append failure text if it is a valid string */
		if( testStatusPtr->text && strlen(testStatusPtr->text) > 0 )
		{
			junit_report_file <<"\t\t\t\t"<< testStatusPtr->text << std::endl;
		}

		/** Append failure xml tag end */
		junit_report_file << "\t\t\t</failure>" << std::endl;
		junit_report_file.close();
}

JUnitReporter::~JUnitReporter() {
	/** As this module is a very tidy one
	 * there is no need to do anything when we deconstruct the object*/
}

