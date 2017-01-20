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

	TEST(IgnoreIndex_OutOfRange_DoesNotThrowException)
	{
		size_t out = pzVowel.values().size();
		pzVowel.ignoreIndex(out);
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

	TEST(Ignore_Getter)
	{
		pzVowel.ignore("E");
		CHECK(pzVowel.isIgnored("E"));
		CHECK(!pzVowel.isIgnored("Y"));
	}

	TEST(IgnoreGlobal_Getter)
	{
		CHECK(pzVowelPartial.isIgnored("E"));
		CHECK(!pzVowelPartial.isIgnored("Y"));
	}

	//////////

	vector<TestListNode*> currentTestsNodes;
	TEST(NoDeadLoopInTestList)
	{
		pzOneTwo(); // Important for perform looping

		for (TestListNode* iNode = Test::GetTestList().GetHead(); iNode != nullptr; iNode = iNode->m_next)
		{
			REQUIRE CHECK(iNode != iNode->m_next);
			currentTestsNodes.push_back(iNode);
		}
	}

	TEST(NoDeadLoopInTestList_Verify)
	{
		vector<TestListNode*> expectedNodes;

		for (size_t i = 0; i < pzOneTwo.values().size(); i++) // tested vector contains repeated values because it is filled by parameterized test
		{
			for (TestListNode* iNode = Test::GetTestList().GetHead(); iNode != nullptr; iNode = iNode->m_next)
			{
				expectedNodes.push_back(iNode);
			}
		}

		REQUIRE CHECK_EQUAL(expectedNodes.size(), currentTestsNodes.size());
		CHECK_ARRAY_EQUAL(expectedNodes, currentTestsNodes, expectedNodes.size());
	}

	//////////

	struct Initializer4pzIgnoredByNameVowel
	{
		Initializer4pzIgnoredByNameVowel()
		{
			ParameterizedManager::getInstance()
				.ignoreIndex("pzIgnoredByNameVowel", 2) // "I"
				.ignoreIndex("pzIgnoredByNameVowel", 5); // "Y"
		}
	} initializer4pzIgnoredByNameVowelInstance;

	SET_TEST_PARAMETER(string, pzIgnoredByNameVowel, {
		values = pzVowel.values();
	});

	string ignoreSomeVowelsByName_Global;
	TEST(Ignore_IgnoreByName_Global)
	{
		ignoreSomeVowelsByName_Global += pzIgnoredByNameVowel();
	}

	TEST(Ignore_IgnoreByName_Global_Verify)
	{
		CHECK_EQUAL("AEOU", ignoreSomeVowelsByName_Global);
	}

	string ignoreSomeVowelsByName_Local;
	TEST(Ignore_IgnoreByName_Local)
	{
		ParameterizedManager::getInstance()
			.ignoreIndex("pzIgnoredByNameVowel", 3); // "O"

		ignoreSomeVowelsByName_Local += pzIgnoredByNameVowel();
	}

	TEST(Ignore_IgnoreByName_Local_Verify)
	{
		CHECK_EQUAL("AEU", ignoreSomeVowelsByName_Local);
	}


	//////////

	SET_TEST_PARAMETER(string, pzIgnoredByNameVowelSome, {
		values = pzVowel.values();
	});

	string ignoreByNameRangeEmptyIsAllowed;
	TEST(Ignore_IgnoreByNameRange_EmptyIsAllowed)
	{
		ParameterizedManager::getInstance().ignoreIndexes("pzIgnoredByNameVowelSome[]");
		ignoreByNameRangeEmptyIsAllowed += pzIgnoredByNameVowelSome();
	}

	TEST(Ignore_IgnoreByNameRange_EmptyIsAllowed_Verify)
	{
		CHECK_EQUAL("AEIOUY", ignoreByNameRangeEmptyIsAllowed);
	}

	string ignoreByNameRangeSingleValue;
	TEST(Ignore_IgnoreByNameRange_SingleValue)
	{
		ParameterizedManager::getInstance().ignoreIndexes("pzIgnoredByNameVowelSome[2]");
		ignoreByNameRangeSingleValue += pzIgnoredByNameVowelSome();
	}

	TEST(Ignore_IgnoreByNameRange_SingleValue_Verify)
	{
		CHECK_EQUAL("AEOUY", ignoreByNameRangeSingleValue);
	}

	string ignoreByNameRangeSeveralUnorderedValues;
	TEST(Ignore_IgnoreByNameRange_SeveralUnorderedValues)
	{
		ParameterizedManager::getInstance().ignoreIndexes("pzIgnoredByNameVowelSome[3,4,0]");
		ignoreByNameRangeSeveralUnorderedValues += pzIgnoredByNameVowelSome();
	}

	TEST(Ignore_IgnoreByNameRange_SeveralUnorderedValues_Verify)
	{
		CHECK_EQUAL("EIY", ignoreByNameRangeSeveralUnorderedValues);
	}
}
