#ifndef UNITTEST_REQUIREMACROS_H
#define UNITTEST_REQUIREMACROS_H

#include "HelperMacros.h"
#include "ExceptionMacros.h"
#include "CurrentTest.h"

#ifdef REQUIRE
    #error UnitTest++ redefines REQUIRE
#endif

#ifndef UNITTEST_NO_EXCEPTIONS
    #define REQUIRE(test) \
        UNITTEST_MULTILINE_MACRO_BEGIN \
            int const failuresBeforeTest = UnitTest::CurrentTest::Results()->GetFailureCount(); \
            test; \
            int const failuresAfterTest = UnitTest::CurrentTest::Results()->GetFailureCount(); \
            if(failuresAfterTest > failuresBeforeTest) \
            { \
                UT_THROW(UnitTest::AssertException()); \
            } \
        UNITTEST_MULTILINE_MACRO_END
    #endif
#endif

#ifdef UNITTEST_NO_EXCEPTIONS
    #define REQUIRE(test) test;
#endif
