#ifndef UNITTEST_TESTREPORTERSTDOUT_H
#define UNITTEST_TESTREPORTERSTDOUT_H

#include "TestReporter.h"

namespace UnitTest {

class UNITTEST_LINKAGE TestReporterStdout : public TestReporter
{
private:
    virtual void ReportTestStart(TestDetails const& test);
    virtual void ReportFailure(TestDetails const& test, char const* failure);
    virtual void ReportTestFinish(TestDetails const& test, float secondsElapsed);
    virtual void ReportSummary(int const totalTestCount, int const failedTestCount, int const failureCount, float const secondsElapsed);
};

}

#endif 
