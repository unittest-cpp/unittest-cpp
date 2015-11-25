#ifndef UNITTEST_CHECKMACROS_H
#define UNITTEST_CHECKMACROS_H

#include "HelperMacros.h"
#include "Checks.h"
#include "AssertException.h"
#include "TestDetails.h"
#include "CurrentTest.h"
#include "ReportAssertImpl.h"
#include "CheckMacroImplementations.h"

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

#define CHECK(value) \
    UNITTEST_CHECK(value, \
                   if (!UnitTest::Check(value)) \
                   { \
                       UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), #value); \
                   })

#define CHECK_DESCRIBED(value, description) \
    UNITTEST_CHECK(value, UNITTEST_CHECK_WITH_DESCRIPTION(UnitTest::Check(value), description))

#define CHECK_EQUAL(expected, actual) \
    UNITTEST_CHECK_EQUAL(expected, actual, \
                         UnitTest::CheckEqual(*UnitTest::CurrentTest::Results(), expected, actual, \
                                              UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)))

#define CHECK_EQUAL_DESCRIBED(expected, actual, description) \
    UNITTEST_CHECK_EQUAL(expected, actual, \
                         UNITTEST_CHECK_WITH_DESCRIPTION(UnitTest::AreEqual(expected, actual), description))

#define CHECK_CLOSE(expected, actual, tolerance) \
    UNITTEST_CHECK_CLOSE(expected, actual, tolerance, \
                         UnitTest::CheckClose(*UnitTest::CurrentTest::Results(), expected, actual, tolerance, \
                                              UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)))

#define CHECK_CLOSE_DESCRIBED(expected, actual, tolerance, description) \
    UNITTEST_CHECK_CLOSE(expected, actual, tolerance, \
                         UNITTEST_CHECK_WITH_DESCRIPTION(UnitTest::AreClose(expected, actual, tolerance), description))

#define CHECK_ARRAY_EQUAL(expected, actual, count) \
    UNITTEST_CHECK_ARRAY_EQUAL(expected, actual, count, \
                               UnitTest::CheckArrayEqual(*UnitTest::CurrentTest::Results(), expected, actual, count, \
                                                         UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)))

#define CHECK_ARRAY_EQUAL_DESCRIBED(expected, actual, count, description) \
    UNITTEST_CHECK_ARRAY_EQUAL(expected, actual, count, \
                               UNITTEST_CHECK_WITH_DESCRIPTION(UnitTest::ArrayAreEqual(expected, actual, count), description))

#define CHECK_ARRAY_CLOSE(expected, actual, count, tolerance) \
    UNITTEST_CHECK_ARRAY_CLOSE(expected, actual, count, tolerance, \
                               UnitTest::CheckArrayClose(*UnitTest::CurrentTest::Results(), expected, actual, count, tolerance, \
                                                         UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)))

#define CHECK_ARRAY_CLOSE_DESCRIBED(expected, actual, count, tolerance, description) \
    UNITTEST_CHECK_ARRAY_CLOSE(expected, actual, count, tolerance, \
                               UNITTEST_CHECK_WITH_DESCRIPTION(UnitTest::ArrayAreClose(expected, actual, count, tolerance), description))

#define CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance) \
    UNITTEST_CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance, \
                                 UnitTest::CheckArray2DClose(*UnitTest::CurrentTest::Results(), expected, actual, rows, columns, tolerance, \
                                                             UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)))

#define CHECK_ARRAY2D_CLOSE_DESCRIBED(expected, actual, rows, columns, tolerance, description) \
    UNITTEST_CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance, \
                                 UNITTEST_CHECK_WITH_DESCRIPTION(UnitTest::Array2DAreClose(expected, actual, rows, columns, tolerance), description))

// CHECK_THROW and CHECK_ASSERT only exist when UNITTEST_NO_EXCEPTIONS isn't defined (see config.h)
#ifndef UNITTEST_NO_EXCEPTIONS

#define CHECK_THROW(expression, ExpectedExceptionType) \
    UNITTEST_CHECK_THROW(expression, ExpectedExceptionType, \
                         UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                                                                         "Expected exception: \"" #ExpectedExceptionType "\" not thrown"))

#define CHECK_THROW_DESCRIBED(expression, ExpectedExceptionType, description) \
    UNITTEST_CHECK_THROW(expression, ExpectedExceptionType, \
                         UNITTEST_CHECK_WITH_DESCRIPTION(false, description))

#define CHECK_ASSERT(expression) \
    UNITTEST_MULTILINE_MACRO_BEGIN \
        UnitTest::Detail::ExpectAssert(true); \
        CHECK_THROW(expression, UnitTest::AssertException); \
        UnitTest::Detail::ExpectAssert(false); \
    UNITTEST_MULTILINE_MACRO_END

#define CHECK_ASSERT_DESCRIBED(expression, description) \
    UNITTEST_MULTILINE_MACRO_BEGIN \
        UnitTest::Detail::ExpectAssert(true); \
        CHECK_THROW_DESCRIBED(expression, UnitTest::AssertException, description); \
        UnitTest::Detail::ExpectAssert(false); \
    UNITTEST_MULTILINE_MACRO_END

#endif
#endif
