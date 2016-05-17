#include "SuitePredicate.h"

using namespace std;
using namespace UnitTest;


SuitePredicate::SuitePredicate()
{
}


SuitePredicate::~SuitePredicate()
{
}


void SuitePredicate::addSuite(char const* suiteName)
{
	_suiteNames.push_back(suiteName);
}


void SuitePredicate::addTest(char const* testName)
{
	_testNames.push_back(testName);
}


bool SuitePredicate::operator()(Test const * const test) const
{
	return (mustExecuteSuite(test) || mustExecuteTest(test));
}


bool SuitePredicate::mustExecuteSuite(Test const * const test) const
{
	for (size_t i = 0; i < _suiteNames.size(); i++)
	{
		if (_suiteNames[i] == test->m_details.suiteName)
		{
			return true;
		}
	}
	return false;
}


bool SuitePredicate::mustExecuteTest(Test const * const test) const
{
	for (size_t i = 0; i < _testNames.size(); i++)
	{
		if (_testNames[i] == test->m_details.testName)
		{
			return true;
		}
	}
	return false;
}
