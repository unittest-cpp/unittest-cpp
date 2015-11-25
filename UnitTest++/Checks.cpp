#include "Checks.h"
#include <cstring>

namespace UnitTest {

namespace {

bool StringsAreEqual(char const* expected, char const* actual)
{
    return !((expected && actual) ? strcmp(expected, actual) : (expected || actual));
}

void CheckStringsEqual(TestResults& results, char const* expected, char const* actual,
                       TestDetails const& details)
{
    using namespace std;

    if (!StringsAreEqual(expected, actual))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected " << (expected ? expected : "<NULLPTR>") << " but was " << (actual ? actual : "<NULLPTR>");

        results.OnTestFailure(details, stream.GetText());
    }
}

}

bool AreEqual(char const* expected, char const* actual)
{
    return StringsAreEqual(expected, actual);
}

void CheckEqual(TestResults& results, char const* expected, char const* actual,
                TestDetails const& details)
{
    CheckStringsEqual(results, expected, actual, details);
}

bool AreEqual(char* expected, char* actual)
{
    return StringsAreEqual(expected, actual);
}

void CheckEqual(TestResults& results, char* expected, char* actual,
                TestDetails const& details)
{
    CheckStringsEqual(results, expected, actual, details);
}
bool AreEqual(char* expected, char const* actual)
{
    return StringsAreEqual(expected, actual);
}

void CheckEqual(TestResults& results, char* expected, char const* actual,
                TestDetails const& details)
{
    CheckStringsEqual(results, expected, actual, details);
}

bool AreEqual(char const* expected, char* actual)
{
    return StringsAreEqual(expected, actual);
}

void CheckEqual(TestResults& results, char const* expected, char* actual,
                TestDetails const& details)
{
    CheckStringsEqual(results, expected, actual, details);
}

}
