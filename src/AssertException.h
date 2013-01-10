#ifndef UNITTEST_ASSERTEXCEPTION_H
#define UNITTEST_ASSERTEXCEPTION_H

#include "../config.h"
#ifndef UNITTEST_NO_EXCEPTIONS

#include "HelperMacros.h"
#include <exception>

namespace UnitTest {

class UNITTEST_LINKAGE AssertException : public std::exception
{
public:
    AssertException();
    virtual ~AssertException() throw();
};

}

#endif

#endif
