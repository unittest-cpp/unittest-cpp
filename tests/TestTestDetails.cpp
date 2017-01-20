#include "UnitTest++/UnitTestPP.h"
#include "UnitTest++/TestDetails.h"


SUITE(TestDetails)
{
	using namespace UnitTest;

	TEST(SameTest_SameValues_AreEqual)
	{
		TestDetails details1("test1", "suite1", "file1", 1234);
		TestDetails details2("test1", "suite1", "file1", 1234);
		CHECK(details1.sameTest(details2));
	}

	TEST(SameTest_DifferentValues_AreNotEqual)
	{
		TestDetails details1("test1", "suite1", "file1", 1234);
		TestDetails details2("test2", "suite2", "file2", 4321);
		CHECK(!details1.sameTest(details2));
	}

	TEST(SameTest_LineNumber_AreIgnored)
	{
		TestDetails details1("test1", "suite1", "file1", 1234);
		TestDetails details2("test1", "suite1", "file1", 0);
		CHECK(details1.sameTest(details2));
	}

	TEST(SameTest_CopyConstructor_AreEqual)
	{
		TestDetails details1("test1", "suite1", "file1", 1234);
		TestDetails details2(details1, 4321);
		CHECK(details1.sameTest(details2));
	}
}
