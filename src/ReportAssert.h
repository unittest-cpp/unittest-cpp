#ifndef UNITTEST_ASSERT_H
#define UNITTEST_ASSERT_H

namespace UnitTest {

class TestResults;
class TestDetails;

void ReportAssert(char const* description, char const* filename, int lineNumber);

void ReportAssertEx(TestResults* testResults, 
					const TestDetails* testDetails,
					char const* description, 
					char const* filename, 
					int lineNumber);

void ExpectAssert(bool expected);
bool AssertExpected();

}

#endif
