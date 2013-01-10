#include "../../config.h"
#ifdef UNITTEST_USE_EXCEPTIONS

#include "../../unittestpp.h"
#include "../CurrentTest.h"
#include "RecordingReporter.h"
#include "ScopedCurrentTest.h"

using namespace std;

namespace {

int ThrowingFunction()
{
    throw "Doh";
}

TEST(CheckFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);
        CHECK(ThrowingFunction() == 1);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckFailureBecauseOfExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);
        CHECK(ThrowingFunction() == 1);
    }

    CHECK(strstr(reporter.lastFailedMessage, "ThrowingFunction() == 1"));
}

TEST(CheckEqualFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);
        CHECK_EQUAL(ThrowingFunction(), 1);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckEqualFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
		UnitTest::TestDetails const testDetails("testName", "suiteName", "filename", -1);
		ScopedCurrentTest scopedResults(testResults, &testDetails);

		CHECK_EQUAL(ThrowingFunction(), 123);    line = __LINE__;
    }

    CHECK_EQUAL("testName", reporter.lastFailedTest);
    CHECK_EQUAL("suiteName", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckEqualFailureBecauseOfExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);
        CHECK_EQUAL(ThrowingFunction(), 123);
    }

    CHECK(strstr(reporter.lastFailedMessage, "ThrowingFunction()"));
    CHECK(strstr(reporter.lastFailedMessage, "123"));
}

TEST(CheckCloseFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);
        CHECK_CLOSE((float)ThrowingFunction(), 1.0001f, 0.1f);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckCloseFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
		UnitTest::TestDetails testDetails("closeTest", "closeSuite", "filename", -1);
		ScopedCurrentTest scopedResults(testResults, &testDetails);
        CHECK_CLOSE((float)ThrowingFunction(), 1.0001f, 0.1f);    line = __LINE__;
    }

    CHECK_EQUAL("closeTest", reporter.lastFailedTest);
    CHECK_EQUAL("closeSuite", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckCloseFailureBecauseOfExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);
        CHECK_CLOSE((float)ThrowingFunction(), 1.0001f, 0.1f);
    }

    CHECK(strstr(reporter.lastFailedMessage, "(float)ThrowingFunction()"));
    CHECK(strstr(reporter.lastFailedMessage, "1.0001f"));
}

class ThrowingObject
{
public:
    float operator[](int) const
    {
        throw "Test throw";
    }
};

TEST(CheckArrayCloseFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
		UnitTest::TestDetails testDetails("arrayCloseTest", "arrayCloseSuite", "filename", -1);
		ScopedCurrentTest scopedResults(testResults, &testDetails);

		int const data[4] = { 0, 1, 2, 3 };
        CHECK_ARRAY_CLOSE(data, ThrowingObject(), 4, 0.01f);     line = __LINE__;
    }

    CHECK_EQUAL("arrayCloseTest", reporter.lastFailedTest);
    CHECK_EQUAL("arrayCloseSuite", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckArrayCloseFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);

		const float data[4] = { 0, 1, 2, 3 };
        ThrowingObject obj;
        CHECK_ARRAY_CLOSE(data, obj, 3, 0.01f);

		failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckArrayCloseFailureOnExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);

		const float data[4] = { 0, 1, 2, 3 };
        ThrowingObject obj;
        CHECK_ARRAY_CLOSE(data, obj, 3, 0.01f);
    }

    CHECK(strstr(reporter.lastFailedMessage, "data"));
    CHECK(strstr(reporter.lastFailedMessage, "obj"));
}

TEST(CheckArrayEqualFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);

		const float data[4] = { 0, 1, 2, 3 };
        ThrowingObject obj;
        CHECK_ARRAY_EQUAL (data, obj, 3);

		failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckArrayEqualFailureOnExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);

		const float data[4] = { 0, 1, 2, 3 };
        ThrowingObject obj;
        CHECK_ARRAY_EQUAL (data, obj, 3);
    }

    CHECK(strstr(reporter.lastFailedMessage, "data"));
    CHECK(strstr(reporter.lastFailedMessage, "obj"));
}

int g_sideEffect = 0;
float const* FunctionWithSideEffects2()
{
    ++g_sideEffect;
    static float const data[] = {1,2,3,4};
    return data;
}

class ThrowingObject2D
{
public:
    float* operator[](int) const
    {
        throw "Test throw";
    }
};

TEST(CheckArray2DCloseFailureBecauseOfExceptionContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
		UnitTest::TestDetails testDetails("array2DCloseTest", "array2DCloseSuite", "filename", -1);
		ScopedCurrentTest scopedResults(testResults, &testDetails);

		const float data[2][2] = { {0, 1}, {2, 3} };
        CHECK_ARRAY2D_CLOSE(data, ThrowingObject2D(), 2, 2, 0.01f);   line = __LINE__;
    }

    CHECK_EQUAL("array2DCloseTest", reporter.lastFailedTest);
    CHECK_EQUAL("array2DCloseSuite", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckArray2DCloseFailsOnException)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);

		const float data[2][2] = { {0, 1}, {2, 3} };
        ThrowingObject2D obj;
        CHECK_ARRAY2D_CLOSE(data, obj, 2, 2, 0.01f);

		failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckArray2DCloseFailureOnExceptionIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
		ScopedCurrentTest scopedResults(testResults);

		const float data[2][2] = { {0, 1}, {2, 3} };
        ThrowingObject2D obj;
        CHECK_ARRAY2D_CLOSE(data, obj, 2, 2, 0.01f);
    }

    CHECK(strstr(reporter.lastFailedMessage, "data"));
    CHECK(strstr(reporter.lastFailedMessage, "obj"));
}

}

#endif
