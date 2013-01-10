#ifndef UNITTEST_REPORTASSERTIMPL_H
#define UNITTEST_REPORTASSERTIMPL_H

#include "../config.h"

#ifndef UNITTEST_USE_EXCEPTIONS
	#include <csetjmp>
#endif

namespace UnitTest {

class TestResults;
class TestDetails;

namespace Detail {

void ExpectAssert(bool expected);

void ReportAssertEx(TestResults* testResults, 
					const TestDetails* testDetails,
					char const* description, 
					char const* filename, 
					int lineNumber);

bool AssertExpected();

#ifndef UNITTEST_USE_EXCEPTIONS
	jmp_buf* GetAssertJmpBuf();

	#ifdef _MSC_VER
		#define UNITTEST_SET_ASSERT_JUMP_TARGET() \
			__pragma(warning(push)) __pragma(warning(disable:4611)) \
			setjmp(*UnitTest::Detail::GetAssertJmpBuf()) \
			__pragma(warning(pop))
	#else
		#define UNITTEST_SET_ASSERT_JUMP_TARGET() setjmp(*UnitTest::Detail::GetAssertJmpBuf())
	#endif

	#define UNITTEST_JUMP_TO_ASSERT_JUMP_TARGET() longjmp(*UnitTest::Detail::GetAssertJmpBuf(), 1)
#endif

}
}

#endif
