#include "UnitTest++/UnitTestPP.h"
#include "UnitTest++/ArgumentsReader.h"

using namespace UnitTest;

SUITE(ArgumentsReader)
{
	TEST(Extract_IgnoreUndesired)
	{
		char full[] = "C:\\myProgram.exe";
		char valFake1[] = "fake";
		char valFake2[] = "fakeBis";
		char* argv[] = { full, valFake1, valFake2 };
		ArgumentsReader args(3, argv);

		CHECK(args.extractValues("--unexisting").empty());
	}

	TEST(Extract_CanExtractImplicitly)
	{
		char full[] = "C:\\myProgram.exe";
		char val1[] = "val1";
		char val2[] = "val2";
		char* argv[] = { full, val1, val2 };
		ArgumentsReader args(3, argv);

		vector<string> values = args.extractValues("");
		REQUIRE CHECK_EQUAL(2, values.size());
		CHECK_EQUAL(val1, values[0]);
		CHECK_EQUAL(val2, values[1]);
	}


	TEST(Extract_ReadSimple)
	{
		char full[] = "C:\\myProgram.exe";
		char paramToto[] = "--toto";
		char val1[] = "myTest";
		char* argv[] = { full, paramToto, val1 };
		ArgumentsReader args(3, argv);

		vector<string> values = args.extractValues(paramToto);
		REQUIRE CHECK_EQUAL(1, values.size());
		CHECK_EQUAL(val1, values[0]);
	}


	TEST(Extract_CanReadMultiple)
	{
		char full[] = "C:\\myProgram.exe";
		char fake[] = "fake";
		char paramToto[] = "--toto";
		char val1[] = "myTest";
		char val2[] = "myOtherTest";
		char* argv[] = { full, fake, paramToto, val1, val2 };
		ArgumentsReader args(5, argv);

		CHECK(args.extractValues("--unexisting").empty());

		vector<string> values = args.extractValues(paramToto);
		REQUIRE CHECK_EQUAL(2, values.size());
		CHECK_EQUAL(val1, values[0]);
		CHECK_EQUAL(val2, values[1]);
	}


	TEST(Extract_BlindTest)
	{
		char full[] = "C:\\myProgram.exe";
		char fake[] = "fake";
		char paramToto[] = "--toto";
		char totoVal1[] = "myTest";
		char totoVal2[] = "myOtherTest";
		char paramTiti[] = "--titi";
		char titiVal[] = "yourTest";
		char* argv[] = { full, fake, paramToto, totoVal1, totoVal2, paramTiti, titiVal };
		ArgumentsReader args(7, argv);

		vector<string> valuesToto = args.extractValues(paramToto);
		REQUIRE CHECK_EQUAL(2, valuesToto.size());
		CHECK_EQUAL(totoVal1, valuesToto[0]);
		CHECK_EQUAL(totoVal2, valuesToto[1]);

		vector<string> valuesTiti = args.extractValues(paramTiti);
		REQUIRE CHECK_EQUAL(1, valuesTiti.size());
		CHECK_EQUAL(titiVal, valuesTiti[0]);
	}
}
