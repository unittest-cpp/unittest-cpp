#include "UnitTest++/UnitTest++.h"

#include <string>
#include "UnitTest++/ParameterizedTest.h"
#include "ScopedCurrentTest.h"
#include "RecordingReporter.h"

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
		CHECK_EQUAL(6, simpleVowelsHitCount);
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
		catch (runtime_error e) // Expected case
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

	SET_SUITE_PARAMETERS(string, oneTwo, {
		parameters.push_back("1");
		parameters.push_back("2");
	});

	string nestedParameters;
	TEST(NestedParameters)
	{
		nestedParameters += vowel();
		nestedParameters += oneTwo();
	}

	TEST(NestedParameters_Verify)
	{
		CHECK_EQUAL("A1A2E1E2I1I2O1O2U1U2Y1Y2", nestedParameters);
	}

	//////////

	string useSeveralTimes;
	TEST(UseSeveralTimes_DoesNotIncrement)
	{
		useSeveralTimes += oneTwo();
		useSeveralTimes += oneTwo();
	}

	TEST(UseSeveralTimes_DoesNotIncrement_Verify)
	{
		CHECK_EQUAL("1122", useSeveralTimes);
	}

	//////////

	struct Fixture {};

	string withFixture;
	TEST_FIXTURE(Fixture, WorksWithFixture)
	{
		withFixture += oneTwo();
	}

	TEST(WorksWithFixture_Verify)
	{
		CHECK_EQUAL("12", withFixture);
	}

	//////////

	SET_SUITE_PARAMETERS(int, parameterizedSingleBis, {
		parameters.push_back(3);
	});

	TEST(FailedMessage_ContainsIndexes)
	{
		class FailingParameterizedTest : public Test
		{
		public:
			FailingParameterizedTest()
				: Test(CurrentTest::Details()->testName, CurrentTest::Details()->suiteName, CurrentTest::Details()->filename) {}
			virtual void RunImpl() const
			{
				parameterizedSingle();
				parameterizedSingleBis();
				REQUIRE CHECK(false);
			}
		};

		RecordingReporter reporter;
		TestResults results(&reporter);
		{
			ScopedCurrentTest scopedResults(results);
			FailingParameterizedTest().Run();
		}

		string expectedFailMessage = "false Parameters: "
			+ parameterizedSingle.getName() + "[0], "
			+ parameterizedSingleBis.getName() + "[0]";

		CHECK_EQUAL(expectedFailMessage, string(reporter.lastFailedMessage));
	}

	//////////

	TEST(ExcludeIndex_OutOfRange_ThrowsException)
	{
		size_t out = vowel.parameters().size();
		CHECK_THROW(vowel.excludeIndex(out), out_of_range);
	}

	TEST(ExcludeIndex_ExcludeAll_ThrowsException)
	{
		size_t last = vowel.parameters().size()-1;
		for (size_t i = 0; i < last; i++)
		{
			vowel.excludeIndex(i);
		}

		CHECK_THROW(vowel.excludeIndex(last), runtime_error);
	}

	//////////

	string excludeSomeVowels;
	TEST(ExcludeIndex_ExcludeSome)
	{
		vowel
			.excludeIndex(1) // "E"
			.excludeIndex(4); // "U"

		excludeSomeVowels += vowel();
	}

	TEST(ExcludeIndex_ExcludeSome_Verify)
	{
		CHECK_EQUAL("AIOY", excludeSomeVowels);
	}

	//////////

	string excludeWithNested;
	TEST(ExcludeIndex_ExcludeWithNested)
	{
		vowel
			.excludeIndex(0) // "A"
			.excludeIndex(2) // "I"
			.excludeIndex(3) // "O"
			.excludeIndex(4); // "U"

		excludeWithNested += vowel();
		excludeWithNested += oneTwo();
	}

	TEST(ExcludeIndex_ExcludeWithNested_Verify)
	{
		CHECK_EQUAL("E1E2Y1Y2", excludeWithNested);
	}
}
