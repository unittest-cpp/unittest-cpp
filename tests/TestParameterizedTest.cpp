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
			values.push_back("A");
			values.push_back("E");
			values.push_back("I");
			values.push_back("O");
			values.push_back("U");
			values.push_back("Y");
		}
		vector<string> values;
	} InitVoyelleInstance;

	
	ParameterizedTest<string> voyelle(InitVoyelleInstance.values);

	
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
