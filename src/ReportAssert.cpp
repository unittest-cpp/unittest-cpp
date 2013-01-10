#include "ReportAssert.h"
#include "AssertException.h"
#include "CurrentTest.h"
#include "TestResults.h"
#include "TestDetails.h"

namespace UnitTest {

namespace
{
	bool& AssertExpectedFlag()
	{
		static bool s_assertExpected = false;
		return s_assertExpected;
	}
}

void ReportAssert(char const* description, char const* filename, int lineNumber)
{
	ReportAssertEx(CurrentTest::Results(), CurrentTest::Details(), description, filename, lineNumber);
}

void ReportAssertEx(TestResults* testResults,
					const TestDetails* testDetails,
					char const* description, 
					char const* filename, 
					int lineNumber)
{
	if (AssertExpectedFlag() == false)
	{
		TestDetails assertDetails(testDetails->testName, testDetails->suiteName, filename, lineNumber);
		testResults->OnTestFailure(assertDetails, description);
	}

	ExpectAssert(false);

#ifdef UNITTEST_USE_EXCEPTIONS
	throw AssertException();
#endif
}

void ExpectAssert(bool expected)
{
	AssertExpectedFlag() = expected;
}

bool AssertExpected()
{
	return AssertExpectedFlag();
}

}
