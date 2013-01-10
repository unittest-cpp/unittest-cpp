#include "AssertException.h"

#ifdef UNITTEST_USE_EXCEPTIONS

#include <cstring>

namespace UnitTest {

AssertException::AssertException(char const* description, char const* filename, int lineNumber)
    : m_lineNumber(lineNumber)
{
	using namespace std;

    strcpy(m_description, description);
    strcpy(m_filename, filename);
}

AssertException::~AssertException()
{
}

char const* AssertException::what() const
{
    return m_description;
}

char const* AssertException::Filename() const
{
    return m_filename;
}

int AssertException::LineNumber() const
{
    return m_lineNumber;
}

}

#endif
