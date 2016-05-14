#include "ParameterizedTest.h"

#include "TestList.h"
#include "CurrentTest.h"

using namespace std;
using namespace UnitTest;


ParameterizedTest::ParameterizedTest()
{
}


ParameterizedTest::~ParameterizedTest()
{
}


Test* const ParameterizedTest::retrieveCurrentTest()
{
	return retrieveTest(CurrentTest::Details());
}


Test* const ParameterizedTest::retrieveTest(TestDetails const * const details)
{
	//TODO This workaround is too far complicated, why not simply add pointer to current test in class CurrentTest ?
	Details2Test::iterator it = _tests.find(details);

	if (it != _tests.end())
	{
		return it->second;
	}

	for(Test* iTest = Test::GetTestList().GetHead() ; iTest!= nullptr ; iTest=iTest->m_nextTest)
	{
		if (&iTest->m_details == details)
		{
			_tests[details] = iTest;
			return iTest;
		}
	}

	throw runtime_error(string("Impossible to retrieve test ") + details->testName);
}
