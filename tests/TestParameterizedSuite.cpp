#include "UnitTest++/UnitTest++.h"

#include "UnitTest++/ParameterizedSuite.h"

using namespace std;
using namespace UnitTest;


static int valuesSum = 0;
static int iterationsSum = 0;
static int ignoredCounter = 0;
static int otherParameterizedSuiteSum = 0;
static int singleValueSuiteSum = 0;
static int noValueSuiteSum = 0;

SUITE(ParameterizedSuite)
{
	TEST(TestsOfBelowAreIgnored)
	{
		ignoredCounter++;
		CHECK_EQUAL(1, ignoredCounter);
	}

	vector<int> values = { 1, 2, 3, 4 };
	ParameterizedSuite<int> parameterized(UnitTestSuite::GetSuiteName(), values);

	size_t lastIteration = -1;
	int lastValue = 0;

	TEST(IterationsAreFollowing)
	{
		CHECK_EQUAL(lastIteration + 1, parameterized.getIteration());
		lastIteration = parameterized.getIteration();
		iterationsSum++;
	}

	TEST(ValuesAreFollowing)
	{
		CHECK_EQUAL(lastValue + 1, parameterized.getCurrent());
		lastValue = parameterized.getCurrent();
		valuesSum += parameterized.getCurrent();
	}

	TEST(ValueAndIterationAreSync)
	{
		CHECK_EQUAL(parameterized.getIteration() + 1, parameterized.getCurrent());
	}

	vector<int> values2 = { 1000, 2000 };
	ParameterizedSuite<int> parameterized2(UnitTestSuite::GetSuiteName(), values2);

	TEST(OtherPSIgnoredFromFirstPS)
	{
		otherParameterizedSuiteSum += parameterized2.getCurrent();
	}
}


SUITE(ParameterizedSuite_LessValues)
{
	vector<int> noValues = {};
	ParameterizedSuite<int> parameterizedEmpty(UnitTestSuite::GetSuiteName(), noValues);

	TEST(WhenNoValue_zeroExecution)
	{
		noValueSuiteSum++;
		throw exception("Should not have been reached");
	}


	vector<int> singleValue = { 2 };
	ParameterizedSuite<int> parameterizedSingle(UnitTestSuite::GetSuiteName(), singleValue);

	TEST(WhenSingleValue_singleExecution)
	{
		singleValueSuiteSum += parameterizedSingle.getCurrent();
		CHECK_EQUAL(2, singleValueSuiteSum);
	}
}


SUITE(ParameterizedSuite_Validation)
{
	TEST(ParameterizedSuiteHasBeenIterated)
	{
		CHECK_EQUAL(1, ignoredCounter);
		CHECK_EQUAL(10, valuesSum);
		CHECK_EQUAL(4, iterationsSum);
		CHECK_EQUAL(3000, otherParameterizedSuiteSum);
		CHECK_EQUAL(2, singleValueSuiteSum);
		CHECK_EQUAL(0, noValueSuiteSum);
	}
}
