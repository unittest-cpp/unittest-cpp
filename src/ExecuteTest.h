#ifndef UNITTEST_EXECUTE_TEST_H
#define UNITTEST_EXECUTE_TEST_H

#include "ExceptionMacros.h"
#include "TestDetails.h"
#include "MemoryOutStream.h"
#include "AssertException.h"
#include "CurrentTest.h"

#ifdef UNITTEST_POSIX
	#include "Posix/SignalTranslator.h"
#endif

namespace UnitTest {

template< typename T >
void ExecuteTest(T& testObject, TestDetails const& details)
{
	CurrentTest::Details() = &details;

#ifndef UNITTEST_POSIX
	UT_TRY
	({
		testObject.RunImpl();
	})
#else
	UT_TRY
	({
		UNITTEST_THROW_SIGNALS_POSIX_ONLY
		testObject.RunImpl();
	})
#endif

	UT_CATCH(AssertException, e,
	{
		CurrentTest::Results()->OnTestFailure(
			TestDetails(details.testName, details.suiteName, e.Filename(), e.LineNumber()), e.what());
	})
	UT_CATCH(std::exception, e,
	{
		MemoryOutStream stream;
		stream << "Unhandled exception: " << e.what();
		CurrentTest::Results()->OnTestFailure(details, stream.GetText());
	})
	UT_CATCH_ALL
	({
		CurrentTest::Results()->OnTestFailure(details, "Unhandled exception: Crash!");
	})
}

}

#endif
