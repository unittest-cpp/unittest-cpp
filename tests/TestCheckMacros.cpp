#include <ios>
#include "UnitTest++/UnitTestPP.h"
#include "UnitTest++/CurrentTest.h"
#include "UnitTest++/Config.h"
#include "RecordingReporter.h"
#include "ScopedCurrentTest.h"

using namespace std;

namespace {

int g_sideEffect = 0;
int FunctionWithSideEffects()
{
    ++g_sideEffect;
    return 1;
}

TEST(CheckSucceedsOnTrue)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);

        ScopedCurrentTest scopedResults(testResults);
        CHECK(true);

        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckFailsOnFalse)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK(false);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(FailureReportsCorrectTestName)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK(false);
    }

    CHECK_EQUAL(m_details.testName, reporter.lastFailedTest);
}

TEST(CheckFailureIncludesCheckContents)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        const bool yaddayadda = false;
        CHECK(yaddayadda);
    }

    CHECK(strstr(reporter.lastFailedMessage, "yaddayadda"));
}

TEST(CheckDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK(FunctionWithSideEffects());
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK(!FunctionWithSideEffects());
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckDescribedSucceedsOnTrue)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);

        ScopedCurrentTest scopedResults(testResults);
        CHECK_DESCRIBED(true, "description");

        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckDescribedFailsOnFalse)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_DESCRIBED(false, "description");
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckDescribedFailureReportsCorrectTestName)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_DESCRIBED(false, "description");
    }

    CHECK_EQUAL(m_details.testName, reporter.lastFailedTest);
}

TEST(CheckDescribedFailureIncludesCustomDescription)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_DESCRIBED(false, "description " << hex << 0x1234);
    }

    CHECK(strstr(reporter.lastFailedMessage, "description 1234"));
}

TEST(CheckDescribedDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_DESCRIBED(FunctionWithSideEffects(), "description");
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckDescribedDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_DESCRIBED(!FunctionWithSideEffects(), "description");
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckEqualSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_EQUAL(1, 1);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckEqualFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_EQUAL(1, 2);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckEqualFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        UnitTest::TestDetails const testDetails("testName", "suiteName", "filename", -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        CHECK_EQUAL(1, 123);    line = __LINE__;
    }

    CHECK_EQUAL("testName", reporter.lastFailedTest);
    CHECK_EQUAL("suiteName", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckEqualDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_EQUAL(1, FunctionWithSideEffects());
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckEqualDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_EQUAL(2, FunctionWithSideEffects());
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckEqualDescribedSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_EQUAL_DESCRIBED(1, 1, "description");
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckEqualDescribedFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_EQUAL_DESCRIBED(1, 2, "description");
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckEqualDescribedFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        UnitTest::TestDetails const testDetails("testName", "suiteName", "filename", -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        CHECK_EQUAL_DESCRIBED(1, 123, "description " << hex << 0x1234);    line = __LINE__;
    }

    CHECK_EQUAL("testName", reporter.lastFailedTest);
    CHECK_EQUAL("suiteName", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL("description 1234", reporter.lastFailedMessage);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckEqualDescribedDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_EQUAL_DESCRIBED(1, FunctionWithSideEffects(), "description");
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckEqualDescribedDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_EQUAL_DESCRIBED(2, FunctionWithSideEffects(), "description");
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckCloseSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_CLOSE (1.0f, 1.001f, 0.01f);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckCloseFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_CLOSE (1.0f, 1.1f, 0.01f);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckCloseFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        UnitTest::TestDetails testDetails("test", "suite", "filename", -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        CHECK_CLOSE (1.0f, 1.1f, 0.01f);    line = __LINE__;
    }

    CHECK_EQUAL("test", reporter.lastFailedTest);
    CHECK_EQUAL("suite", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckCloseDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_CLOSE (1, FunctionWithSideEffects(), 0.1f);
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckCloseDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_CLOSE (2, FunctionWithSideEffects(), 0.1f);
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckCloseDescribedSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_CLOSE_DESCRIBED(1.0f, 1.001f, 0.01f, "description");
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckCloseDescribedFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_CLOSE_DESCRIBED(1.0f, 1.1f, 0.01f, "description");
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckCloseDescribedFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        UnitTest::TestDetails testDetails("test", "suite", "filename", -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        CHECK_CLOSE_DESCRIBED(1.0f, 1.1f, 0.01f, "description " << hex << 0x1234);    line = __LINE__;
    }

    CHECK_EQUAL("test", reporter.lastFailedTest);
    CHECK_EQUAL("suite", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL("description 1234", reporter.lastFailedMessage);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckCloseDescribedDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_CLOSE_DESCRIBED(1, FunctionWithSideEffects(), 0.1f, "description");
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckCloseDescribedDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_CLOSE_DESCRIBED(2, FunctionWithSideEffects(), 0.1f, "description");
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckArrayEqualSuceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[4] = { 0, 1, 2, 3 };
        CHECK_ARRAY_EQUAL (data, data, 4);

        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckArrayEqualFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        CHECK_ARRAY_EQUAL (data1, data2, 4);

        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckArrayEqualFailureIncludesCheckExpectedAndActual)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        CHECK_ARRAY_EQUAL (data1, data2, 4);
    }

    CHECK(strstr(reporter.lastFailedMessage, "xpected [ 0 1 2 3 ]"));
    CHECK(strstr(reporter.lastFailedMessage, "was [ 0 1 3 3 ]"));
}

TEST(CheckArrayEqualFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        UnitTest::TestDetails testDetails("arrayEqualTest", "arrayEqualSuite", "filename", -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        CHECK_ARRAY_EQUAL (data1, data2, 4);     line = __LINE__;
    }

    CHECK_EQUAL("arrayEqualTest", reporter.lastFailedTest);
    CHECK_EQUAL("arrayEqualSuite", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

float const* FunctionWithSideEffects2()
{
    ++g_sideEffect;
    static float const data[] = {1,2,3,4};
    return data;
}

TEST(CheckArrayEqualDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[] = { 0, 1, 2, 3 };
        CHECK_ARRAY_EQUAL (data, FunctionWithSideEffects2(), 4);
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckArrayEqualDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[] = { 0, 1, 3, 3 };
        CHECK_ARRAY_EQUAL (data, FunctionWithSideEffects2(), 4);
    }

    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckArrayEqualDescribedSuceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[4] = { 0, 1, 2, 3 };
        CHECK_ARRAY_EQUAL_DESCRIBED(data, data, 4, "description");

        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckArrayEqualDescribedFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        CHECK_ARRAY_EQUAL_DESCRIBED(data1, data2, 4, "description");

        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckArrayEqualDescribedFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        UnitTest::TestDetails testDetails("arrayEqualTest", "arrayEqualSuite", "filename", -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        CHECK_ARRAY_EQUAL_DESCRIBED(data1, data2, 4, "description " << hex << 0x1234);     line = __LINE__;
    }

    CHECK_EQUAL("arrayEqualTest", reporter.lastFailedTest);
    CHECK_EQUAL("arrayEqualSuite", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL("description 1234", reporter.lastFailedMessage);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckArrayEqualDescribedDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[] = { 0, 1, 2, 3 };
        CHECK_ARRAY_EQUAL_DESCRIBED(data, FunctionWithSideEffects2(), 4, "description");
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckArrayEqualDescribedDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[] = { 0, 1, 3, 3 };
        CHECK_ARRAY_EQUAL_DESCRIBED(data, FunctionWithSideEffects2(), 4, "description");
    }

    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckArrayCloseSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        const float data[4] = { 0, 1, 2, 3 };
        CHECK_ARRAY_CLOSE (data, data, 4, 0.01f);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckArrayCloseFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        CHECK_ARRAY_CLOSE (data1, data2, 4, 0.01f);

        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckArrayCloseFailureIncludesCheckExpectedAndActual)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        CHECK_ARRAY_CLOSE (data1, data2, 4, 0.01f);
    }

    CHECK(strstr(reporter.lastFailedMessage, "xpected [ 0 1 2 3 ]"));
    CHECK(strstr(reporter.lastFailedMessage, "was [ 0 1 3 3 ]"));
}

TEST(CheckArrayCloseFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        UnitTest::TestDetails testDetails("arrayCloseTest", "arrayCloseSuite", "filename", -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        int const data1[4] = { 0, 1, 2, 3 };
        int const data2[4] = { 0, 1, 3, 3 };
        CHECK_ARRAY_CLOSE (data1, data2, 4, 0.01f);     line = __LINE__;
    }

    CHECK_EQUAL("arrayCloseTest", reporter.lastFailedTest);
    CHECK_EQUAL("arrayCloseSuite", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckArrayCloseFailureIncludesTolerance)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        float const data1[4] = { 0, 1, 2, 3 };
        float const data2[4] = { 0, 1, 3, 3 };
        CHECK_ARRAY_CLOSE (data1, data2, 4, 0.01f);
    }

    CHECK(strstr(reporter.lastFailedMessage, "0.01"));
}

TEST(CheckArrayCloseDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[] = { 0, 1, 2, 3 };
        CHECK_ARRAY_CLOSE (data, FunctionWithSideEffects2(), 4, 0.01f);
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckArrayCloseDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[] = { 0, 1, 3, 3 };
        CHECK_ARRAY_CLOSE (data, FunctionWithSideEffects2(), 4, 0.01f);
    }

    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckArray2DCloseSucceedsOnEqual)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        const float data[2][2] = { {0, 1}, {2, 3} };
        CHECK_ARRAY2D_CLOSE (data, data, 2, 2, 0.01f);

        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckArray2DCloseFailsOnNotEqual)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[2][2] = { {0, 1}, {2, 3} };
        int const data2[2][2] = { {0, 1}, {3, 3} };
        CHECK_ARRAY2D_CLOSE (data1, data2, 2, 2, 0.01f);

        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckArray2DCloseFailureIncludesCheckExpectedAndActual)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        int const data1[2][2] = { {0, 1}, {2, 3} };
        int const data2[2][2] = { {0, 1}, {3, 3} };

        CHECK_ARRAY2D_CLOSE (data1, data2, 2, 2, 0.01f);
    }

    CHECK(strstr(reporter.lastFailedMessage, "xpected [ [ 0 1 ] [ 2 3 ] ]"));
    CHECK(strstr(reporter.lastFailedMessage, "was [ [ 0 1 ] [ 3 3 ] ]"));
}

TEST(CheckArray2DCloseFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        UnitTest::TestDetails testDetails("array2DCloseTest", "array2DCloseSuite", "filename", -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        int const data1[2][2] = { {0, 1}, {2, 3} };
        int const data2[2][2] = { {0, 1}, {3, 3} };
        CHECK_ARRAY2D_CLOSE (data1, data2, 2, 2, 0.01f);     line = __LINE__;
    }

    CHECK_EQUAL("array2DCloseTest", reporter.lastFailedTest);
    CHECK_EQUAL("array2DCloseSuite", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckArray2DCloseFailureIncludesTolerance)
{
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        float const data1[2][2] = { {0, 1}, {2, 3} };
        float const data2[2][2] = { {0, 1}, {3, 3} };
        CHECK_ARRAY2D_CLOSE (data1, data2, 2, 2, 0.01f);
    }

    CHECK(strstr(reporter.lastFailedMessage, "0.01"));
}

float const* const* FunctionWithSideEffects3()
{
    ++g_sideEffect;
    static float const data1[] = {0,1};
    static float const data2[] = {2,3};
    static const float* const data[] = {data1, data2};
    return data;
}

TEST(CheckArray2DCloseDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[2][2] = { {0, 1}, {2, 3} };
        CHECK_ARRAY2D_CLOSE (data, FunctionWithSideEffects3(), 2, 2, 0.01f);
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckArray2DCloseDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);

        const float data[2][2] = { {0, 1}, {3, 3} };
        CHECK_ARRAY2D_CLOSE (data, FunctionWithSideEffects3(), 2, 2, 0.01f);
    }
    CHECK_EQUAL(1, g_sideEffect);
}


// CHECK_THROW and CHECK_ASSERT only exist when UNITTEST_NO_EXCEPTIONS isn't defined (see config.h)
#ifndef UNITTEST_NO_EXCEPTIONS
void ThrowInt()
{
    throw 42;
}

void ThrowNothing()
{
}

void ThrowIntWithSideEffect()
{
    ++g_sideEffect;
    ThrowInt();
}

TEST(CheckThrowSucceedsWhenCorrectTypeTrown)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_THROW(ThrowInt(), int);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckThrowFailsWhenNothingThrown)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_THROW(ThrowNothing(), int);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckThrowFailsWhenWrongTypeThrown)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_THROW(ThrowInt(), float);
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckThrowFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        UnitTest::TestDetails const testDetails("testName", "suiteName", "filename", -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        CHECK_THROW(ThrowInt(), float);    line = __LINE__;
    }

    CHECK_EQUAL("testName", reporter.lastFailedTest);
    CHECK_EQUAL("suiteName", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL("Expected exception: \"float\" not thrown", reporter.lastFailedMessage);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckThrowDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_THROW(ThrowIntWithSideEffect(), int);
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckThrowDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_THROW(ThrowIntWithSideEffect(), float);
    }
    CHECK_EQUAL(1, g_sideEffect);
}

void Assert()
{
    UnitTest::ReportAssert("description", "filename", __LINE__);
}

void NoAssert()
{
}

void AssertWithSideEffect()
{
    ++g_sideEffect;
    UnitTest::ReportAssert("description", "filename", __LINE__);
}

TEST(CheckAssertSucceedsWhenAssertThrown)
{
    bool failure = true;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_ASSERT(Assert());
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(!failure);
}

TEST(CheckAssertFailsWhenAssertNotThrown)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_ASSERT(NoAssert());
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckAssertFailsWhenWrongTypeThrown)
{
    bool failure = false;
    {
        RecordingReporter reporter;
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);
        CHECK_ASSERT(ThrowInt());
        failure = (testResults.GetFailureCount() > 0);
    }

    CHECK(failure);
}

TEST(CheckAssertFailureContainsCorrectDetails)
{
    int line = 0;
    RecordingReporter reporter;
    {
        UnitTest::TestResults testResults(&reporter);
        UnitTest::TestDetails const testDetails("testName", "suiteName", "filename", -1);
        ScopedCurrentTest scopedResults(testResults, &testDetails);

        CHECK_ASSERT(ThrowInt());    line = __LINE__;
    }

    CHECK_EQUAL("testName", reporter.lastFailedTest);
    CHECK_EQUAL("suiteName", reporter.lastFailedSuite);
    CHECK_EQUAL("filename", reporter.lastFailedFile);
    CHECK_EQUAL("Expected exception: \"UnitTest::AssertException\" not thrown", reporter.lastFailedMessage);
    CHECK_EQUAL(line, reporter.lastFailedLine);
}

TEST(CheckAssertDoesNotHaveSideEffectsWhenPassing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_ASSERT(AssertWithSideEffect());
    }
    CHECK_EQUAL(1, g_sideEffect);
}

TEST(CheckAssertDoesNotHaveSideEffectsWhenFailing)
{
    g_sideEffect = 0;
    {
        UnitTest::TestResults testResults;
        ScopedCurrentTest scopedResults(testResults);
        CHECK_ASSERT(ThrowIntWithSideEffect());
    }
    CHECK_EQUAL(1, g_sideEffect);
}

#endif  // UNITTEST_NO_EXCEPTIONS

}
