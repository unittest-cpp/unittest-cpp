#include "UnitTest++/UnitTest++.h"

#include <string>
#include "UnitTest++/ParameterizedTest.h"

using namespace std;
using namespace UnitTest;


SUITE(ParameterizedTestSimple)
{
	string simpleVowels;
	int simpleVowelsHitCount = 0;

	SET_SUITE_PARAMETERS(string, vowel, {
		parameters.push_back("A");
		parameters.push_back("E");
		parameters.push_back("I");
		parameters.push_back("O");
		parameters.push_back("U");
		parameters.push_back("Y");
	});

	TEST(SimpleVoyelConcat)
	{
		simpleVowels += vowel();
		simpleVowelsHitCount++;
	}

	TEST(SimpleVoyelConcat_Verify)
	{
		CHECK_EQUAL(parameterizedCreatorvowelInstance.parameters.size(), simpleVowelsHitCount);
		CHECK_EQUAL("AEIOUY", simpleVowels);
	}

	//////////

	int hitCountNonParameterized = 0;
	TEST(NonParameterizedExecutedOnce)
	{
		hitCountNonParameterized++;
		CHECK_EQUAL(1, hitCountNonParameterized);
	}

	//////////

	bool enteredEmpty = false;
	int hitCountEmpty = 0;

	SET_SUITE_PARAMETERS(int, parameterizedEmpty, {
	});

	TEST(WhenNoParameters_throwsException)
	{
		enteredEmpty = true;
		try
		{
			parameterizedEmpty();
		}
		catch(runtime_error e) // Expected case
		{
			CHECK_EQUAL("No values for parameterized test", e.what());
			return;
		}

		CHECK(false);
		hitCountEmpty++;
	}

	TEST(WhenNoParameters_throwsException_Verify)
	{
		CHECK_EQUAL(0, hitCountEmpty);
		CHECK(enteredEmpty);
	}

	//////////

	int hitCountSingle = 0;
	static int singleValueSuiteSum = 0;

	SET_SUITE_PARAMETERS(int, parameterizedSingle, {
		parameters.push_back(2);
	});

	TEST(WhenSingleValue_singleExecution)
	{
		singleValueSuiteSum += parameterizedSingle();
		CHECK_EQUAL(2, singleValueSuiteSum);
		hitCountSingle++;
	}

	TEST(WhenSingleValue_singleExecution_Verification)
	{
		CHECK_EQUAL(2, singleValueSuiteSum);
		CHECK_EQUAL(1, hitCountSingle);
	}

	//////////

	string voyelReuse;
	TEST(ReusePreviousParameterized)
	{
		voyelReuse += vowel() + "-"; // Add a separator for 
	}

	TEST(ReusePreviousParameterized_Verify)
	{
		CHECK_EQUAL("A-E-I-O-U-Y-", voyelReuse);
	}

	//////////

	SET_SUITE_PARAMETERS(string, xyz, {
		parameters.push_back("X");
		parameters.push_back("Y");
		parameters.push_back("Z");
	});

	string nestedParameters;
	TEST(NestedParameters)
	{
		nestedParameters += vowel() + xyz(); // Add a separator for 
	}

	TEST(NestedParameters_Verify)
	{
		CHECK_EQUAL("AXEXIXOXUXYXAYEYIYOYUYYYAZEZIZOZUZYZ", nestedParameters);
	}
}
