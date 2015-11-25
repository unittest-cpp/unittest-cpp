#ifndef UNITTEST_REQUIREMACROS_H
#define UNITTEST_REQUIREMACROS_H

#include "RequiredCheckTestReporter.h"

#ifdef REQUIRE
    #error UnitTest++ redefines REQUIRE
#endif

#ifndef UNITTEST_NO_EXCEPTIONS
    #define REQUIRE for(UnitTest::RequiredCheckTestReporter decoratedReporter(UnitTest::CurrentTest::Results()); decoratedReporter.next(); )
#endif

#ifdef UNITTEST_NO_EXCEPTIONS
    #define REQUIRE
#endif

#endif