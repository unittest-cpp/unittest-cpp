#ifndef UNITTEST_ASSERTEXCEPTION_H
#define UNITTEST_ASSERTEXCEPTION_H

#include "../config.h"
#ifdef UNITTEST_USE_EXCEPTIONS

#include <exception>

namespace UnitTest {

class AssertException : public std::exception
{
public:
    AssertException();
    virtual ~AssertException() throw();
};

}

#endif

#endif
