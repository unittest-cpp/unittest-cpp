#ifndef UNITTEST_CHECKMACROS_H
#define UNITTEST_CHECKMACROS_H

#include "HelperMacros.h"
#include "ExceptionMacros.h"
#include "Checks.h"
#include "AssertException.h"
#include "MemoryOutStream.h"
#include "TestDetails.h"
#include "CurrentTest.h"
#include "ReportAssertImpl.h"

#ifdef CHECK
    #error UnitTest++ redefines CHECK
#endif

#ifdef CHECK_EQUAL
    #error UnitTest++ redefines CHECK_EQUAL
#endif

#ifdef CHECK_CLOSE
    #error UnitTest++ redefines CHECK_CLOSE
#endif

#ifdef CHECK_ARRAY_EQUAL
    #error UnitTest++ redefines CHECK_ARRAY_EQUAL
#endif

#ifdef CHECK_ARRAY_CLOSE
    #error UnitTest++ redefines CHECK_ARRAY_CLOSE
#endif

#ifdef CHECK_ARRAY2D_CLOSE
    #error UnitTest++ redefines CHECK_ARRAY2D_CLOSE
#endif

#define UNITTEST_CHECK_WITH_DESCRIPTION(condition, description) \
    if (!condition) \
    { \
        UnitTest::MemoryOutStream stream; \
        stream << description; \
        UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), stream.GetText()); \
    }

#define CHECK(value) \
    UNITTEST_CHECK(value, \
                   if (!UnitTest::Check(value)) \
                   { \
                       UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), #value); \
                   })

#define CHECK_DESCRIBED(value, description) \
    UNITTEST_CHECK(value, UNITTEST_CHECK_WITH_DESCRIPTION(UnitTest::Check(value), description))

#define UNITTEST_CHECK(value, test_action) \
    UNITTEST_MULTILINE_MACRO_BEGIN \
    UT_TRY \
        ({ \
            test_action; \
        }) \
        UT_CATCH (std::exception, e, \
        { \
            UnitTest::MemoryOutStream message; \
            message << "Unhandled exception (" << e.what() << ") in CHECK(" #value ")"; \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                message.GetText()); \
        }) \
        UT_CATCH_ALL \
        ({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                "Unhandled exception in CHECK(" #value ")"); \
        }) \
    UNITTEST_MULTILINE_MACRO_END

#define CHECK_EQUAL(expected, actual) \
    UNITTEST_CHECK_EQUAL(expected, actual, \
                         UnitTest::CheckEqual(*UnitTest::CurrentTest::Results(), expected, actual, \
                                              UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)))

#define UNITTEST_CHECK_EQUAL(expected, actual, test_action) \
    UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
        ({ \
            test_action; \
        }) \
        UT_CATCH (std::exception, e, \
        { \
            UnitTest::MemoryOutStream message; \
            message << "Unhandled exception (" << e.what() << ") in CHECK_EQUAL(" #expected ", " #actual ")"; \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                message.GetText()); \
        }) \
        UT_CATCH_ALL \
        ({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")"); \
        }) \
    UNITTEST_MULTILINE_MACRO_END

#define CHECK_CLOSE(expected, actual, tolerance) \
    UNITTEST_CHECK_CLOSE(expected, actual, tolerance, \
                         UnitTest::CheckClose(*UnitTest::CurrentTest::Results(), expected, actual, tolerance, \
                                              UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)))

#define UNITTEST_CHECK_CLOSE(expected, actual, tolerance, test_action)      \
    UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
        ({ \
            test_action; \
        }) \
        UT_CATCH (std::exception, e, \
        { \
            UnitTest::MemoryOutStream message; \
            message << "Unhandled exception (" << e.what() << ") in CHECK_CLOSE(" #expected ", " #actual ")"; \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                message.GetText()); \
        }) \
        UT_CATCH_ALL \
        ({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_CLOSE(" #expected ", " #actual ")"); \
        }) \
    UNITTEST_MULTILINE_MACRO_END

#define CHECK_ARRAY_EQUAL(expected, actual, count) \
    UNITTEST_CHECK_ARRAY_EQUAL(expected, actual, count, \
                               UnitTest::CheckArrayEqual(*UnitTest::CurrentTest::Results(), expected, actual, count, \
                                                         UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)))

#define UNITTEST_CHECK_ARRAY_EQUAL(expected, actual, count, test_action)    \
    UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
        ({ \
            test_action; \
        }) \
        UT_CATCH (std::exception, e, \
        { \
            UnitTest::MemoryOutStream message; \
            message << "Unhandled exception (" << e.what() << ") in CHECK_ARRAY_EQUAL(" #expected ", " #actual ")"; \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                message.GetText()); \
        }) \
        UT_CATCH_ALL \
        ({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_ARRAY_EQUAL(" #expected ", " #actual ")"); \
        }) \
    UNITTEST_MULTILINE_MACRO_END

#define CHECK_ARRAY_CLOSE(expected, actual, count, tolerance) \
    UNITTEST_CHECK_ARRAY_CLOSE(expected, actual, count, tolerance, \
                               UnitTest::CheckArrayClose(*UnitTest::CurrentTest::Results(), expected, actual, count, tolerance, \
                                                         UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)))

#define UNITTEST_CHECK_ARRAY_CLOSE(expected, actual, count, tolerance, test_action) \
    UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
        ({ \
            test_action; \
        }) \
        UT_CATCH (std::exception, e, \
        { \
            UnitTest::MemoryOutStream message; \
            message << "Unhandled exception (" << e.what() << ") in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"; \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                message.GetText()); \
        }) \
        UT_CATCH_ALL \
        ({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"); \
        }) \
    UNITTEST_MULTILINE_MACRO_END

#define CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance) \
    UNITTEST_CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance, \
                                 UnitTest::CheckArray2DClose(*UnitTest::CurrentTest::Results(), expected, actual, rows, columns, tolerance, \
                                                             UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)))

#define UNITTEST_CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance, test_action) \
    UNITTEST_MULTILINE_MACRO_BEGIN \
        UT_TRY \
        ({ \
            test_action; \
        }) \
        UT_CATCH (std::exception, e, \
        { \
            UnitTest::MemoryOutStream message; \
            message << "Unhandled exception (" << e.what() << ") in CHECK_ARRAY2D_CLOSE(" #expected ", " #actual ")"; \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                message.GetText()); \
        }) \
        UT_CATCH_ALL \
        ({ \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_ARRAY2D_CLOSE(" #expected ", " #actual ")"); \
        }) \
    UNITTEST_MULTILINE_MACRO_END

// CHECK_THROW and CHECK_ASSERT only exist when UNITTEST_NO_EXCEPTIONS isn't defined (see config.h)
#ifndef UNITTEST_NO_EXCEPTIONS

#define CHECK_THROW(expression, ExpectedExceptionType) \
    UNITTEST_CHECK_THROW(expression, ExpectedExceptionType, \
                         UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                                                                         "Expected exception: \"" #ExpectedExceptionType "\" not thrown"))

#define UNITTEST_CHECK_THROW(expression, ExpectedExceptionType, failure_action)        \
    UNITTEST_MULTILINE_MACRO_BEGIN \
        bool caught_ = false; \
        try { expression; } \
        catch (ExpectedExceptionType const&) { caught_ = true; } \
        catch (...) {} \
        if (!caught_) \
        { \
            failure_action; \
        } \
    UNITTEST_MULTILINE_MACRO_END

#define CHECK_ASSERT(expression) \
    UNITTEST_MULTILINE_MACRO_BEGIN \
        UnitTest::Detail::ExpectAssert(true); \
        CHECK_THROW(expression, UnitTest::AssertException); \
        UnitTest::Detail::ExpectAssert(false); \
    UNITTEST_MULTILINE_MACRO_END
#endif
#endif
