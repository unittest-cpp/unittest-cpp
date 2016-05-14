#include "UnitTest++/UnitTest++.h"

#include <string>
#include "UnitTest++/ParameterizedTest.h"

using namespace std;
using namespace UnitTest;

SUITE(ParameterizedTest)
{
	vector<string> voyelleValues;
	ParameterizedTest<string> voyelle(voyelleValues);

	struct InitVoyelle
	{
		InitVoyelle()
		{
			voyelleValues.push_back("A");
			voyelleValues.push_back("E");
			voyelleValues.push_back("I");
			voyelleValues.push_back("O");
			voyelleValues.push_back("U");
			voyelleValues.push_back("Y");
		}
	} InitVoyelleInstance;

	
	static string buildVoyellesStringOutput;
	TEST(BuildVoyellesString)
	{
		buildVoyellesStringOutput += voyelle();
	}

	TEST(CheckVoyelle)
	{
		CHECK_EQUAL("AEIOUY", buildVoyellesStringOutput);
	}
}
