#include <ios>
#include "UnitTest++/UnitTestPP.h"
#include "ScopedCurrentTest.h"

// These are sample tests that show the different features of the framework

namespace {

TEST(ValidCheckSucceeds)
{
    bool const b = true;
    CHECK(b);
}

TEST(CheckWorksWithPointers)
{
    void* p = (void *)0x100;
    CHECK(p);
    CHECK(p != 0);
}

TEST(CanUseCheckDescribedToGetCustomFailureMessage)
{
    bool const b = true;
    CHECK_DESCRIBED(b, "b is not " << std::boolalpha << true << ", b is " << b);
}

TEST(ValidCheckEqualSucceeds)
{
    int const x = 3;
    int const y = 3;
    CHECK_EQUAL(x, y);
}

TEST(CheckEqualWorksWithPointers)
{
    void* p = (void *)0;
    CHECK_EQUAL((void*)0, p);
}

TEST(CanUseCheckEqualsDescribedToGetCustomFailureMessage)
{
    int const x = 0x44;
    int const expected = 0x44;
    CHECK_EQUAL_DESCRIBED(expected, x, "x is not " << std::showbase << std::hex << expected << ", x is " << x);
}

TEST(ValidCheckCloseSucceeds)
{
    CHECK_CLOSE(2.0f, 2.001f, 0.01f);
    CHECK_CLOSE(2.001f, 2.0f, 0.01f);
}

TEST(ArrayEqualSucceeds)
{
    int const a1[] = {1, 2, 3};
    int const a2[] = {1, 2, 3};
    CHECK_ARRAY_EQUAL(a1, a2, 3);
}

TEST(ArrayCloseSucceeds)
{
    float const a1[] = {1, 2, 3};
    float const a2[] = {1, 2.01f, 3};
    CHECK_ARRAY_CLOSE(a1, a2, 3, 0.1f);
}

TEST(Array2dCloseSucceeds)
{
    float const a1[][3] = {{1, 2, 3}, {4, 5, 6}};
    float const a2[][3] = {{1, 2.01f, 3}, {4, 5.01, 6}};
    CHECK_ARRAY2D_CLOSE(a1, a2, 2, 3, 0.1f);
}

#ifndef UNITTEST_NO_EXCEPTIONS

TEST(CheckThrowMacroSucceedsOnCorrectException)
{
    struct TestException {};
    CHECK_THROW(throw TestException(), TestException);
}

TEST(CheckAssertSucceeds)
{
    CHECK_ASSERT(UnitTest::ReportAssert("desc", "file", 0));
}

TEST(CheckThrowMacroFailsOnMissingException)
{
    class NoThrowTest : public UnitTest::Test
    {
    public:
        NoThrowTest() : Test("nothrow") {}
        void DontThrow() const
        {
        }

        virtual void RunImpl() const
        {
            CHECK_THROW(DontThrow(), int);
        }
    };

    UnitTest::TestResults results;
    {
        ScopedCurrentTest scopedResults(results);

        NoThrowTest test;
        test.Run();
    }

    CHECK_EQUAL(1, results.GetFailureCount());
}

TEST(CheckThrowMacroFailsOnWrongException)
{
    class WrongThrowTest : public UnitTest::Test
    {
    public:
        WrongThrowTest() : Test("wrongthrow") {}
        virtual void RunImpl() const
        {
            CHECK_THROW(throw "oops", int);
        }
    };

    UnitTest::TestResults results;
    {
        ScopedCurrentTest scopedResults(results);

        WrongThrowTest test;
        test.Run();
    }

    CHECK_EQUAL(1, results.GetFailureCount());
}

#endif

struct SimpleFixture
{
    SimpleFixture()
    {
        ++instanceCount;
    }
    ~SimpleFixture()
    {
        --instanceCount;
    }

    static int instanceCount;
};

int SimpleFixture::instanceCount = 0;

TEST_FIXTURE(SimpleFixture, DefaultFixtureCtorIsCalled)
{
    CHECK(SimpleFixture::instanceCount > 0);
}

TEST_FIXTURE(SimpleFixture, OnlyOneFixtureAliveAtATime)
{
    CHECK_EQUAL(1, SimpleFixture::instanceCount);
}

void CheckBool(const bool b)
{
    CHECK(b);
}

TEST(CanCallCHECKOutsideOfTestFunction)
{
    CheckBool(true);
}

}
