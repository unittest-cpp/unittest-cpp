#ifndef UNITTEST_REQUIREMACROS_H
#define UNITTEST_REQUIREMACROS_H

#include "HelperMacros.h"
#include "ExceptionMacros.h"
#include "CurrentTest.h"

#ifdef REQUIRE
    #error UnitTest++ redefines REQUIRE
#endif

#ifndef UNITTEST_NO_EXCEPTIONS
    #define REQUIRE \
            for (int failuresBeforeTest = UnitTest::CurrentTest::Results()->GetFailureCount(), newFailures = 0, run = 0; \
                (run == 0) || ((newFailures != 0) && (throw UnitTest::AssertException(), true)); \
                newFailures = UnitTest::CurrentTest::Results()->GetFailureCount() - failuresBeforeTest, run = 1)
#endif

#ifdef UNITTEST_NO_EXCEPTIONS
    #define REQUIRE
#endif

#endif