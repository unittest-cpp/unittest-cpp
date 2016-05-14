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


Details2Test & ParameterizedTest::getFastSearchMap()
{
	static Details2Test storedMap;
	return storedMap;
}


Test* const ParameterizedTest::retrieveCurrentTest()
{
	return retrieveTest(CurrentTest::Details());
}


Test* const ParameterizedTest::retrieveTest(TestDetails const * const details)
{
	//TODO This workaround is too far complicated, why not simply add pointer to current test in class CurrentTest ?
	Details2Test::iterator it = getFastSearchMap().find(details);

	if (it != getFastSearchMap().end())
	{
		return it->second;
	}

	for(Test* iTest = Test::GetTestList().GetHead() ; iTest!= nullptr ; iTest=iTest->m_nextTest)
	{
		if (&iTest->m_details == details)
		{
			getFastSearchMap()[details] = iTest;
			return iTest;
		}
	}

	return nullptr;
}
