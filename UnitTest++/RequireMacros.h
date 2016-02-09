#ifndef UNITTEST_REQUIREMACROS_H
#define UNITTEST_REQUIREMACROS_H

#include "RequiredCheckTestReporter.h"

#ifdef REQUIRE
    #error UnitTest++ redefines REQUIRE
#endif

#define REQUIRE for(UnitTest::RequiredCheckTestReporter decoratedReporter(*UnitTest::CurrentTest::Results()); decoratedReporter.Next(); )

#endif
