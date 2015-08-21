#ifndef UNITTEST_CHECKMACROIMPLEMENTATIONS_H
#define UNITTEST_CHECKMACROIMPLEMENTATIONS_H

#include "HelperMacros.h"
#include "ExceptionMacros.h"
#include "Checks.h"
#include "MemoryOutStream.h"
#include "TestDetails.h"
#include "CurrentTest.h"

#define UNITTEST_CHECK_WITH_DESCRIPTION(condition, description) \
    if (!condition) \
    { \
        UnitTest::MemoryOutStream stream; \
        stream << description; \
        UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), stream.GetText()); \
    }

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

#ifndef UNITTEST_NO_EXCEPTIONS

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

#endif

#endif
