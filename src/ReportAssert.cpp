#include "ReportAssert.h"
#include "AssertException.h"

namespace UnitTest {

void ReportAssert(char const* description, char const* filename, int lineNumber)
{
	(void)description;
	(void)filename;
	(void)lineNumber;

#ifdef UNITTEST_USE_EXCEPTIONS
    throw AssertException(description, filename, lineNumber);
#endif
}

}
