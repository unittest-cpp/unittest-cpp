#include "UnitTest++/UnitTestPP.h"
#include "UnitTest++/SuitePredicate.h"

SUITE(SuitePredicate)
{
	using namespace UnitTest;


	TEST(Operator_DetectsTests)
	{
		SuitePredicate predicate;
		predicate.addTest("test1");
		predicate.addTest("testFake");

		Test test1("test1", "suite1");
		Test test2("test2");

		CHECK(predicate.operator()(&test1));
		CHECK(!predicate.operator()(&test2));
	}


	TEST(Operator_DetectsSuites)
	{
		SuitePredicate predicate;
		predicate.addSuite("suite1");
		predicate.addSuite("suiteFake");

		Test test1("test1", "suite1");
		Test test2("test2");

		CHECK(predicate.operator()(&test1));
		CHECK(!predicate.operator()(&test2));
	}


	TEST(Operator_WhenAddAll_AllAreAdded)
	{
		SuitePredicate predicate;
		predicate.addAll();

		Test test1("test1", "suite1");
		Test test2("test2");

		CHECK(predicate.operator()(&test1));
		CHECK(predicate.operator()(&test2));
	}

	TEST(AddMultiple_AndCheckWithGetters)
	{
		vector<string> tests;
		tests.push_back("test1");
		tests.push_back("test2");

		vector<string> suites;
		suites.push_back("suite1");
		suites.push_back("suite2");

		SuitePredicate predicate;
		predicate.addTests(tests);
		predicate.addSuites(suites);

		REQUIRE CHECK_EQUAL(tests.size(), predicate.getTests().size());
		CHECK_ARRAY_EQUAL(tests, predicate.getTests(), tests.size());

		REQUIRE CHECK_EQUAL(suites.size(), predicate.getSuites().size());
		CHECK_ARRAY_EQUAL(suites, predicate.getSuites(), suites.size());
	}


	TEST(EmptyPredicate_IsFlaggedAsEmpty)
	{
		SuitePredicate predicate;
		CHECK(predicate.empty());
	}


	TEST(EmptyPredicate_WhenTestAdded_IsFlaggedAsNonEmpty)
	{
		SuitePredicate predicate;
		predicate.addTest("toto");
		CHECK(!predicate.empty());
	}


	TEST(EmptyPredicate_WhenSuiteAdded_IsFlaggedAsNonEmpty)
	{
		SuitePredicate predicate;
		predicate.addSuite("toto");
		CHECK(!predicate.empty());
	}


	TEST(EmptyPredicate_WhenAllAdded_IsFlaggedAsNonEmpty)
	{
		SuitePredicate predicate;
		predicate.addAll();
		CHECK(!predicate.empty());
	}
}
