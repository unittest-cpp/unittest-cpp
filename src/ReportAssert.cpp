#include "ReportAssert.h"
#include "ReportAssertImpl.h"
#include "AssertException.h"
#include "CurrentTest.h"
#include "TestResults.h"
#include "TestDetails.h"

#ifndef UNITTEST_USE_EXCEPTIONS
	#include "ReportAssertImpl.h"
#endif

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
	Detail::ReportAssertEx(CurrentTest::Results(), CurrentTest::Details(), 
						   description, filename, lineNumber);
}

namespace Detail {

#ifndef UNITTEST_USE_EXCEPTIONS
jmp_buf* GetAssertJmpBuf()
{
	static jmp_buf s_jmpBuf;
	return &s_jmpBuf;
}
#endif
	
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
#else
	UNITTEST_JUMP_TO_ASSERT_JUMP_TARGET();
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

}}
