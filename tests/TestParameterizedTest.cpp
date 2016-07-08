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

	SET_TEST_PARAMETER(string, pzVowel, {
		values.push_back("A");
		values.push_back("E");
		values.push_back("I");
		values.push_back("O");
		values.push_back("U");
		values.push_back("Y");
	});

	TEST(SimpleVowelConcat)
	{
		simpleVowels += pzVowel();
		simpleVowelsHitCount++;
	}

	TEST(SimpleVowelConcat_Verify)
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

	SET_TEST_PARAMETER(int, pzEmpty, {});

	TEST(WhenNoParameters_throwsException)
	{
		enteredEmpty = true;
		try
		{
			pzEmpty();
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

	SET_TEST_PARAMETER(int, pzSingle, {
		values.push_back(2);
	});

	TEST(WhenSingleValue_singleExecution)
	{
		singleValueSuiteSum += pzSingle();
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
		voyelReuse += pzVowel() + "-"; // Add a separator for 
	}

	TEST(ReusePreviousParameterized_Verify)
	{
		CHECK_EQUAL("A-E-I-O-U-Y-", voyelReuse);
	}

	//////////

	SET_TEST_PARAMETER(string, pzOneTwo, {
		values.push_back("1");
		values.push_back("2");
	});

	string nestedParameters;
	TEST(NestedParameters)
	{
		nestedParameters += pzVowel();
		nestedParameters += pzOneTwo();
	}

	TEST(NestedParameters_Verify)
	{
		CHECK_EQUAL("A1A2E1E2I1I2O1O2U1U2Y1Y2", nestedParameters);
	}

	//////////

	string useSeveralTimes;
	TEST(UseSeveralTimes_DoesNotIncrement)
	{
		useSeveralTimes += pzOneTwo();
		useSeveralTimes += pzOneTwo();
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
		withFixture += pzOneTwo();
	}

	TEST(WorksWithFixture_Verify)
	{
		CHECK_EQUAL("12", withFixture);
	}

	//////////

	SET_TEST_PARAMETER(int, pzSingleBis, {
		values.push_back(3);
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
				pzSingle();
				pzSingleBis();
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
			+ pzSingle.getName() + "[0], "
			+ pzSingleBis.getName() + "[0]";

		CHECK_EQUAL(expectedFailMessage, string(reporter.lastFailedMessage));
	}

	//////////

	TEST(IgnoreIndex_OutOfRange_ThrowsException)
	{
		size_t out = pzVowel.values().size();
		CHECK_THROW(pzVowel.ignoreIndex(out), out_of_range);
	}

	TEST(IgnoreIndex_IgnoreAll_ThrowsException)
	{
		size_t last = pzVowel.values().size() - 1;
		for (size_t i = 0; i < last; i++)
		{
			pzVowel.ignoreIndex(i);
		}

		CHECK_THROW(pzVowel.ignoreIndex(last), runtime_error);
	}

	//////////

	size_t ignoreIndexLast_count = 0;
	string ignoreIndexLast;
	TEST(IgnoreIndex_IgnoreLast)
	{
		size_t lastIndex = pzOneTwo.values().size() - 1;
		pzOneTwo.ignoreIndex(lastIndex);

		ignoreIndexLast += pzOneTwo();

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
		pzVowel
			.ignoreIndex(1) // "E"
			.ignoreIndex(4); // "U"

		ignoreSomeVowels += pzVowel();
	}

	TEST(IgnoreIndex_IgnoreSome_Verify)
	{
		CHECK_EQUAL("AIOY", ignoreSomeVowels);
	}

	//////////

	string ignoreWithNested;
	TEST(IgnoreIndex_IgnoreWithNested)
	{
		pzVowel
			.ignoreIndex(0) // "A"
			.ignoreIndex(2) // "I"
			.ignoreIndex(3) // "O"
			.ignoreIndex(4); // "U"

		ignoreWithNested += pzVowel();
		ignoreWithNested += pzOneTwo();
	}

	TEST(IgnoreIndex_IgnoreWithNested_Verify)
	{
		CHECK_EQUAL("E1E2Y1Y2", ignoreWithNested);
	}

	//////////

	TEST(IgnoreIndex_IgnoreAfterIterationBegan_ThrowsException)
	{
		pzVowel.ignoreIndex(0);
		pzVowel();
		CHECK_THROW(pzVowel.ignoreIndex(1), runtime_error);
	}

	//////////

	string ignoreSomeVowelsByValue;
	TEST(Ignore_IgnoreSome)
	{
		pzVowel
			.ignore("E")
			.ignore("U");

		ignoreSomeVowelsByValue += pzVowel();
	}

	TEST(Ignore_IgnoreSome_Verify)
	{
		CHECK_EQUAL("AIOY", ignoreSomeVowelsByValue);
	}

	//////////

	SET_TEST_PARAMETER(string, pzVowelPartial, {
		values = pzVowel.values();
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

	//////////

	TEST(LoopIsNotDetectableInTestList)
	{
		pzOneTwo(); // Important for perform looping

		for (TestListNode* iNode = Test::GetTestList().GetHead(); iNode != nullptr; iNode = iNode->m_next)
		{
			REQUIRE CHECK(iNode != iNode->m_next);
		}
	}
}
