#include "UnitTest++/UnitTest++.h"

#include "UnitTest++/ParameterizedSuite.h"

using namespace std;
using namespace UnitTest;

static int simpleValuesSum = 0;
static int valuesSum = 0;
static int iterationsSum = 0;
static int ignoredCounter = 0;
static int otherParameterizedSuiteSum = 0;
static int singleValueSuiteSum = 0;
static int noValueSuiteSum = 0;


PARAMETERIZED_SUITE(ParameterizedSuiteSimple, iVal, int, {
	parameters.push_back(10);
	parameters.push_back(20);
	parameters.push_back(30);
	parameters.push_back(40);
})
{
	TEST(SimpleSum)
	{
		simpleValuesSum += iVal();
	}
}


SUITE(ParameterizedSuite)
{
	TEST(TestsOfBelowAreIgnored)
	{
		ignoredCounter++;
		CHECK_EQUAL(1, ignoredCounter);
	}

	SET_SUITE_PARAMETERS(parameterized, int, {
		parameters.push_back(1);
		parameters.push_back(2);
		parameters.push_back(3);
		parameters.push_back(4);
	})

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


	SET_SUITE_PARAMETERS(parameterized2, int, {
		parameters.push_back(1000);
		parameters.push_back(2000);
	})

	TEST(OtherPSIgnoredFromFirstPS)
	{
		otherParameterizedSuiteSum += parameterized2.getCurrent();
	}
}


SUITE(ParameterizedSuite_LessValues)
{
	SET_SUITE_PARAMETERS(parameterizedEmpty, int, {
	})

	TEST(WhenNoValue_zeroExecution)
	{
		noValueSuiteSum++;
		throw exception("Should not have been reached");
	}

	SET_SUITE_PARAMETERS(parameterizedSingle, int, {
		parameters.push_back(2);
	})

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
		CHECK_EQUAL(100, simpleValuesSum);
		CHECK_EQUAL(1, ignoredCounter);
		CHECK_EQUAL(10, valuesSum);
		CHECK_EQUAL(4, iterationsSum);
		CHECK_EQUAL(3000, otherParameterizedSuiteSum);
		CHECK_EQUAL(2, singleValueSuiteSum);
		CHECK_EQUAL(0, noValueSuiteSum);
	}
}
