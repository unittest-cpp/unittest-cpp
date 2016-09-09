#include "SuitePredicate.h"

using namespace std;
using namespace UnitTest;


SuitePredicate::SuitePredicate()
	: _all(false)
{
}


SuitePredicate::~SuitePredicate()
{
}


void SuitePredicate::addSuite(const string & suiteName)
{
	_suiteNames.push_back(suiteName);
}


void SuitePredicate::addTest(const string & testName)
{
	_testNames.push_back(testName);
}


void SuitePredicate::addSuites(const vector<string> & suiteNames)
{
	for (size_t i = 0; i < suiteNames.size(); i++)
	{
		addSuite(suiteNames[i]);
	}
}


void SuitePredicate::addTests(const vector<string> & testNames)
{
	for (size_t i = 0; i < testNames.size(); i++)
	{
		addTest(testNames[i]);
	}
}


void SuitePredicate::addAll()
{
	_all = true;
}


bool SuitePredicate::empty()
{
	if (!_suiteNames.empty())
	{
		return false;
	}
	if (!_testNames.empty())
	{
		return false;
	}
	if (_all)
	{
		return false;
	}
	return true;
}


bool SuitePredicate::operator()(Test const * const test) const
{
	if (_all)
	{
		return true;
	}
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
