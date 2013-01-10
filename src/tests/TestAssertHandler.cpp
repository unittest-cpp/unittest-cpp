#include "../Config.h"

#include "../unittestpp.h"
#include "../ReportAssert.h"
#include "../AssertException.h"

#include "RecordingReporter.h"

using namespace UnitTest;

namespace {

TEST(CanSetAssertExpected)
{
	ExpectAssert(true);
	CHECK(AssertExpected());

	ExpectAssert(false);
	CHECK(!AssertExpected());
}

#ifdef UNITTEST_USE_EXCEPTIONS

TEST(ReportAssertThrowsAssertException)
{
    bool caught = false;

    try
    {
		TestResults testResults;
		TestDetails testDetails("", "", "", 0);
        ReportAssertEx(&testResults, &testDetails, "", "", 0);
    }
    catch(AssertException const&)
    {
        caught = true;
    }

    CHECK(true == caught);
}

TEST(ReportAssertClearsExpectAssertFlag)
{
	RecordingReporter reporter;
	TestResults testResults(&reporter);
	TestDetails testDetails("", "", "", 0);

	try
	{
		ExpectAssert(true);
		ReportAssertEx(&testResults, &testDetails, "", "", 0);
	}
	catch(AssertException const&)
	{
	}

	CHECK(AssertExpected() == false);
	CHECK_EQUAL(0, reporter.testFailedCount);
}

TEST(ReportAssertWritesFailureToResultsAndDetailsWhenAssertIsNotExpected)
{
    const int lineNumber = 12345;
    const char* description = "description";
    const char* filename = "filename";

	RecordingReporter reporter;
	TestResults testResults(&reporter);
	TestDetails testDetails("", "", "", 0);

    try
    {
        ReportAssertEx(&testResults, &testDetails, description, filename, lineNumber);
    }
    catch(AssertException const&)
    {
    }

	CHECK_EQUAL(description, reporter.lastFailedMessage);
	CHECK_EQUAL(filename, reporter.lastFailedFile);
	CHECK_EQUAL(lineNumber, reporter.lastFailedLine);
}

TEST(ReportAssertReportsNoErrorsWhenAssertIsExpected)
{
	ExpectAssert(true);

	RecordingReporter reporter;
	TestResults testResults(&reporter);
	TestDetails testDetails("", "", "", 0);

	try
	{
		ReportAssertEx(&testResults, &testDetails, "", "", 0);
	}
	catch(AssertException const&)
	{
	}

	CHECK_EQUAL(0, reporter.testFailedCount);
}

#endif

TEST(CheckAssertMacroSetsAssertExpectationToFalseAfterRunning)
{
	CHECK_ASSERT(ReportAssert("", "", 0));
	CHECK(!AssertExpected());
}

}
