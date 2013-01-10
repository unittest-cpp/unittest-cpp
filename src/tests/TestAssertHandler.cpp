#include "../Config.h"

#ifdef UNITTEST_USE_EXCEPTIONS

#include "../unittestpp.h"
#include "../AssertException.h"
#include "../ReportAssert.h"

using namespace UnitTest;

namespace {

TEST(ReportAssertThrowsAssertException)
{
    bool caught = false;

    try
    {
        ReportAssert("", "", 0);
    }
    catch(AssertException const&)
    {
        caught = true;
    }

    CHECK (true == caught);
}

TEST(ReportAssertSetsCorrectInfoInException)
{
    const int lineNumber = 12345;
    const char* description = "description";
    const char* filename = "filename";

    try
    {
        ReportAssert(description, filename, lineNumber);
    }
    catch(AssertException const& e)
    {
        CHECK_EQUAL(description, e.what());
        CHECK_EQUAL(filename, e.Filename());
        CHECK_EQUAL(lineNumber, e.LineNumber());
    }
}

}

#endif