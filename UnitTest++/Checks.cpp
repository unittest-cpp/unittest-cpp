#include "Checks.h"
#include <cstring>

namespace UnitTest {

namespace {

void CheckStringsEqual(TestResults& results, char const* expected, char const* actual, 
                       TestDetails const& details, bool const required)
{
	using namespace std;

    if ((expected && actual) ? strcmp(expected, actual) : (expected || actual))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected " << (expected ? expected : "<NULLPTR>") << " but was " << (actual ? actual : "<NULLPTR>");

        results.OnTestFailure(details, stream.GetText());
        
        if(required)
        {
            UT_THROW(RequiredCheckFailedException());
        }
    }
}

}


void CheckEqual(TestResults& results, char const* expected, char const* actual,
                TestDetails const& details, bool const required)
{
    CheckStringsEqual(results, expected, actual, details, required);
}

void CheckEqual(TestResults& results, char* expected, char* actual,
                TestDetails const& details, bool const required)
{
    CheckStringsEqual(results, expected, actual, details, required);
}

void CheckEqual(TestResults& results, char* expected, char const* actual,
                TestDetails const& details, bool const required)
{
    CheckStringsEqual(results, expected, actual, details, required);
}

void CheckEqual(TestResults& results, char const* expected, char* actual,
                TestDetails const& details, bool const required)
{
    CheckStringsEqual(results, expected, actual, details, required);
}


}
