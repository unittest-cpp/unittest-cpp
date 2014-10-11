#ifndef UNITTEST_REQUIREDCHECKFAILEDEXCEPTION_H
#define UNITTEST_REQUIREDCHECKFAILEDEXCEPTION_H

#include "Config.h"
#ifndef UNITTEST_NO_EXCEPTIONS

#include "HelperMacros.h"
#include <exception>

namespace UnitTest {

class UNITTEST_LINKAGE RequiredCheckFailedException : public std::exception
{
public:
    RequiredCheckFailedException();
    virtual ~RequiredCheckFailedException() throw();
};

}

#endif

#endif
