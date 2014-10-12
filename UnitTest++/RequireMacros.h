#ifndef UNITTEST_REQUIREMACROS_H
#define UNITTEST_REQUIREMACROS_H

#include "HelperMacros.h"
#include "ExceptionMacros.h"
#include "Checks.h"
#include "CurrentTest.h"
#include "AssertException.h"
#include "MemoryOutStream.h"
#include "TestDetails.h"
#include "CurrentTest.h"
#include "ReportAssertImpl.h"
#include "RequiredCheckFailedException.h"

#ifdef REQUIRE_CHECK
    #error UnitTest++ redefines REQUIRE_CHECK
#endif

#ifdef REQUIRE_EQUAL
	#error UnitTest++ redefines REQUIRE_EQUAL
#endif

#ifdef REQUIRE_CLOSE
	#error UnitTest++ redefines REQUIRE_CLOSE
#endif

#ifdef REQUIRE_ARRAY_EQUAL
	#error UnitTest++ redefines REQUIRE_ARRAY_EQUAL
#endif

#ifdef REQUIRE_ARRAY_CLOSE
	#error UnitTest++ redefines REQUIRE_ARRAY_CLOSE
#endif

#ifdef REQUIRE_ARRAY2D_CLOSE
	#error UnitTest++ redefines REQUIRE_ARRAY2D_CLOSE
#endif

#ifdef IS_REQUIRED_CHECK
    #error UnitTest++ redefines IS_REQUIRED_CHECK
#endif 

#define IS_REQUIRED_CHECK true

#define REQUIRE_CHECK(value) \
	UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
		({ \
			if (!UnitTest::Check(value)) \
            { \
				UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), #value); \
                UT_THROW(UnitTest::RequiredCheckFailedException()); \
            } \
		}) \
        UT_CATCH (UnitTest::RequiredCheckFailedException, e, \
        { \
            UT_THROW(e); \
        }) \
		UT_CATCH (std::exception, e, \
		{ \
			UnitTest::MemoryOutStream message; \
			message << "Unhandled exception (" << e.what() << ") in REQUIRE_CHECK(" #value ")"; \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
				message.GetText()); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
		}) \
		UT_CATCH_ALL \
		({ \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
				"Unhandled exception in REQUIRE_CHECK(" #value ")"); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
		}) \
	UNITTEST_MULTILINE_MACRO_END

#define REQUIRE_EQUAL(expected, actual) \
	UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
		({ \
            UnitTest::CheckEqual(*UnitTest::CurrentTest::Results(), expected, actual, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), IS_REQUIRED_CHECK); \
        }) \
        UT_CATCH (UnitTest::RequiredCheckFailedException, e, \
        { \
            UT_THROW(e); \
        }) \
		UT_CATCH (std::exception, e, \
		{ \
			UnitTest::MemoryOutStream message; \
			message << "Unhandled exception (" << e.what() << ") in REQUIRE_EQUAL(" #expected ", " #actual ")"; \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
				message.GetText()); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
		}) \
        UT_CATCH_ALL \
		({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in REQUIRE_EQUAL(" #expected ", " #actual ")"); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
        }) \
	UNITTEST_MULTILINE_MACRO_END

#define REQUIRE_CLOSE(expected, actual, tolerance) \
	UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
		({ \
            UnitTest::CheckClose(*UnitTest::CurrentTest::Results(), expected, actual, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), IS_REQUIRED_CHECK); \
        }) \
        UT_CATCH (UnitTest::RequiredCheckFailedException, e, \
        { \
            UT_THROW(e); \
        }) \
		UT_CATCH (std::exception, e, \
		{ \
			UnitTest::MemoryOutStream message; \
			message << "Unhandled exception (" << e.what() << ") in REQUIRE_CLOSE(" #expected ", " #actual ")"; \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
				message.GetText()); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
		}) \
        UT_CATCH_ALL \
		({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in REQUIRE_CLOSE(" #expected ", " #actual ")"); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
        }) \
	UNITTEST_MULTILINE_MACRO_END

#define REQUIRE_ARRAY_EQUAL(expected, actual, count) \
	UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
		({ \
            UnitTest::CheckArrayEqual(*UnitTest::CurrentTest::Results(), expected, actual, count, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), IS_REQUIRED_CHECK); \
        }) \
        UT_CATCH (UnitTest::RequiredCheckFailedException, e, \
        { \
            UT_THROW(e); \
        }) \
 		UT_CATCH (std::exception, e, \
		{ \
			UnitTest::MemoryOutStream message; \
			message << "Unhandled exception (" << e.what() << ") in REQUIRE_ARRAY_EQUAL(" #expected ", " #actual ")"; \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
				message.GetText()); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
		}) \
        UT_CATCH_ALL \
		({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in REQUIRE_ARRAY_EQUAL(" #expected ", " #actual ")"); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
        }) \
	UNITTEST_MULTILINE_MACRO_END

#define REQUIRE_ARRAY_CLOSE(expected, actual, count, tolerance) \
	UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
		({ \
            UnitTest::CheckArrayClose(*UnitTest::CurrentTest::Results(), expected, actual, count, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), IS_REQUIRED_CHECK); \
        }) \
        UT_CATCH (UnitTest::RequiredCheckFailedException, e, \
        { \
            UT_THROW(e); \
        }) \
 		UT_CATCH (std::exception, e, \
		{ \
			UnitTest::MemoryOutStream message; \
			message << "Unhandled exception (" << e.what() << ") in REQUIRE_ARRAY_CLOSE(" #expected ", " #actual ")"; \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
				message.GetText()); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
		}) \
        UT_CATCH_ALL \
		({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in REQUIRE_ARRAY_CLOSE(" #expected ", " #actual ")"); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
        }) \
	UNITTEST_MULTILINE_MACRO_END

#define REQUIRE_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance) \
	UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
		({ \
            UnitTest::CheckArray2DClose(*UnitTest::CurrentTest::Results(), expected, actual, rows, columns, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), IS_REQUIRED_CHECK); \
        }) \
        UT_CATCH (UnitTest::RequiredCheckFailedException, e, \
        { \
            UT_THROW(e); \
        }) \
 		UT_CATCH (std::exception, e, \
		{ \
			UnitTest::MemoryOutStream message; \
			message << "Unhandled exception (" << e.what() << ") in REQUIRE_ARRAY2D_CLOSE(" #expected ", " #actual ")"; \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
				message.GetText()); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
		}) \
        UT_CATCH_ALL \
		({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in REQUIRE_ARRAY2D_CLOSE(" #expected ", " #actual ")"); \
            UT_THROW(UnitTest::RequiredCheckFailedException()); \
        }) \
	UNITTEST_MULTILINE_MACRO_END


// REQUIRE_THROW and CHECK_ASSERT only exist when UNITTEST_NO_EXCEPTIONS isn't defined (see config.h)
#ifndef UNITTEST_NO_EXCEPTIONS
#define REQUIRE_THROW(expression, ExpectedExceptionType) \
	UNITTEST_MULTILINE_MACRO_BEGIN \
        bool caught_ = false; \
        try { expression; } \
        catch (ExpectedExceptionType const&) { caught_ = true; } \
		catch (...) {} \
        if (!caught_) \
        { \
	        UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), "Expected exception: \"" #ExpectedExceptionType "\" not thrown"); \
            UT_THROW(RequiredCheckFailed); \
        } \
	UNITTEST_MULTILINE_MACRO_END


#define REQUIRE_ASSERT(expression) \
	UNITTEST_MULTILINE_MACRO_BEGIN \
		UnitTest::Detail::ExpectAssert(true); \
		REQUIRE_THROW(expression, UnitTest::AssertException); \
		UnitTest::Detail::ExpectAssert(false); \
	UNITTEST_MULTILINE_MACRO_END
#endif
#endif
