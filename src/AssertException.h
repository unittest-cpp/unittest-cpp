#ifndef UNITTEST_ASSERTEXCEPTION_H
#define UNITTEST_ASSERTEXCEPTION_H

#include "Config.h"

#ifdef UNITTEST_USE_EXCEPTIONS

#include <exception>

namespace UnitTest {

class AssertException : public std::exception
{
public:
    AssertException(char const* description, char const* filename, int lineNumber);
    virtual ~AssertException();

    virtual char const* what() const;

    char const* Filename() const;
    int LineNumber() const;

private:
    char m_description[512];
    char m_filename[256];
    int m_lineNumber;
};

}

#endif

#endif
