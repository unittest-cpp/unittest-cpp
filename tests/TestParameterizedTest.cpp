#include "UnitTest++/UnitTest++.h"

#include <string>
#include "UnitTest++/TestParameter.h"
#include "UnitTest++/ParameterizedManager.h"
#include "ScopedCurrentTest.h"
#include "RecordingReporter.h"

using namespace std;
using namespace UnitTest;


SUITE(ParameterizedTest)
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

	TEST(IgnoreIndex_OutOfRange_ThrowsException)
	{
		size_t out = vowel.parameters().size();
		CHECK_THROW(vowel.ignoreIndex(out), out_of_range);
	}

	TEST(IgnoreIndex_IgnoreAll_ThrowsException)
	{
		size_t last = vowel.parameters().size() - 1;
		for (size_t i = 0; i < last; i++)
		{
			vowel.ignoreIndex(i);
		}

		CHECK_THROW(vowel.ignoreIndex(last), runtime_error);
	}

	//////////

	size_t ignoreIndexLast_count = 0;
	string ignoreIndexLast;
	TEST(IgnoreIndex_IgnoreLast)
	{
		size_t lastIndex = oneTwo.parameters().size() - 1;
		oneTwo.ignoreIndex(lastIndex);

		ignoreIndexLast += oneTwo();

		// WARNING: this is not the test itself, it is only for reveal a "dead loop"
		REQUIRE CHECK(ignoreIndexLast_count < lastIndex);
		ignoreIndexLast_count++;
	}

	TEST(IgnoreIndex_IgnoreLast_Verify)
	{
		CHECK_EQUAL("1", ignoreIndexLast);
	}

	//////////

	string ignoreSomeVowels;
	TEST(IgnoreIndex_IgnoreSome)
	{
		vowel
			.ignoreIndex(1) // "E"
			.ignoreIndex(4); // "U"

		ignoreSomeVowels += vowel();
	}

	TEST(IgnoreIndex_IgnoreSome_Verify)
	{
		CHECK_EQUAL("AIOY", ignoreSomeVowels);
	}

	//////////

	string ignoreWithNested;
	TEST(IgnoreIndex_IgnoreWithNested)
	{
		vowel
			.ignoreIndex(0) // "A"
			.ignoreIndex(2) // "I"
			.ignoreIndex(3) // "O"
			.ignoreIndex(4); // "U"

		ignoreWithNested += vowel();
		ignoreWithNested += oneTwo();
	}

	TEST(IgnoreIndex_IgnoreWithNested_Verify)
	{
		CHECK_EQUAL("E1E2Y1Y2", ignoreWithNested);
	}

	//////////

	TEST(IgnoreIndex_IgnoreAfterIterationBegan_ThrowsException)
	{
		vowel.ignoreIndex(0);
		vowel();
		CHECK_THROW(vowel.ignoreIndex(1), runtime_error);
	}

	//////////

	string ignoreSomeVowelsByValue;
	TEST(Ignore_IgnoreSome)
	{
		vowel
			.ignore("E")
			.ignore("U");

		ignoreSomeVowelsByValue += vowel();
	}

	TEST(Ignore_IgnoreSome_Verify)
	{
		CHECK_EQUAL("AIOY", ignoreSomeVowelsByValue);
	}

	//////////

	SET_SUITE_PARAMETERS(string, pzVowelPartial, {
		parameters = vowel.parameters();
	});

	struct Initializer
	{
		Initializer()
		{
			ParameterizedManager::getInstance()
				.ignoreIndex(&pzVowelPartial, 1) // "E"
				.ignoreIndex(&pzVowelPartial, 4); // "U"
		}
	} initializerInstance;

	string globalIgnoreSomeVowels;

	// Note: the test is executed twice (A/B)
	TEST(GlobalIgnore_IgnoreSome_A)
	{
		globalIgnoreSomeVowels += pzVowelPartial();
	}
	TEST(GlobalIgnore_IgnoreSome_B)
	{
		globalIgnoreSomeVowels += pzVowelPartial();
	}

	TEST(GlobalIgnore_IgnoreSome_Verify)
	{
		CHECK_EQUAL("AIOYAIOY", globalIgnoreSomeVowels);
	}
}
