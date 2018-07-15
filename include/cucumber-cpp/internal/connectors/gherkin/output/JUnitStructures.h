/*
 * JUnitStructures.h
 *
 *  Created on: 1 Kas 2017
 *      Author: z003en0j
 */

#ifndef INC_JUNITSTRUCTURES_H_
#define INC_JUNITSTRUCTURES_H_

#include <ctime>


#define JUNIT_SEVERITY_FATAL "FATAL"
#define JUNIT_SEVERITY_HIGH "HIGH"
#define JUNIT_SEVERITY_MEDIUM "MEDIUM"
#define JUNIT_SEVERITY_LOW "LOW"
typedef struct
{
	/** message: The source code file, the line number, and the rule that is violated. */
	char* msg;
	/** type: The severity of the rule.*/
	char* type;
	/** Text:
			The text of the rule and the severity.
			The analysis provider and the analysis category.
			The source code file.
			The line number*/
	char* text;
}JUnitTestStatus_t;

typedef struct
{
	/** id: The ID of the rule. */
	unsigned int id;
	/** name: The label of the rule. */
	char* name;
	/** time The time that was required to process all the applications of this rule. */
	float time_in_sec;
	/** text: any text you want to attach */
	char* text;
	/** The total number of test cases in test_case_list*/
	unsigned int num_of_test_status;
	/** Child elements: <JUnitTestStatus> */
	JUnitTestStatus_t* test_status_list;
}JUnitTestCase_t;

typedef struct _JUnitTestSuite_t
{
	/** id: The ID of the test suite. */
	unsigned int id;
	/** name: The label of the test suite. */
	char* name;
	/** tests: number of rules or test-cases in test suite */
	unsigned int num_of_tests;
	/** failures: number of failures in test suite */
	unsigned int num_of_failures;
	/** time The time that was required to process the rules in the provider. */
	float time_in_sec;
	/** text: any text you want to attach */
	char* text;
	/** The total number of test cases in test_case_list*/
	unsigned int num_of_test_cases;
	/** Child elements: <JUnitTestCase> */
	JUnitTestCase_t* test_case_list;
}JUnitTestSuite_t;

typedef struct
{
	/** id: The ID of the scan. */
	unsigned int id;
	/** name: The label of the scan */
	char* name;
	/** tests: The total number of rules that were applied. */
	unsigned int num_of_tests;
	/** failures: The total number of rule violations. */
	unsigned int num_of_failures;
	/** time The time that was required to process all the rules. */
	float time_in_sec;
	/** text: any text you want to attach */
	char* text;
	/** The total number of test suites in test_suite_list*/
	unsigned int num_of_test_suites;
	/** test suite list */
	JUnitTestSuite_t* test_suite_list;
}JUnitTestSuites_t;



#endif /* INC_JUNITSTRUCTURES_H_ */
