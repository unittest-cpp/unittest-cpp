#include "ParameterizedTest.h"

#include "TestList.h"
#include "CurrentTest.h"

using namespace std;
using namespace UnitTest;


ParameterizedTestAbstract::ParameterizedTestAbstract()
	: _iteration(0),
	_testAnchor(nullptr),
	_lastTest(nullptr),
	_nextTestBackup(nullptr)
{
}


ParameterizedTestAbstract::~ParameterizedTestAbstract()
{
	if (_testAnchor != nullptr)
	{
		delete _testAnchor;
		_testAnchor = nullptr;
	}
}


size_t ParameterizedTestAbstract::getIteration()
{
	ensureInitialized();
	return _iteration;
}


Test* const ParameterizedTestAbstract::getLastTest() const
{
	return _lastTest;
}


void ParameterizedTestAbstract::ensureInitialized()
{
	Test* currentTest = retrieveCurrentTest();

	if (_testAnchor == nullptr)
	{
		_testAnchor = new TestAnchor("ParameterizedTestAnchor", currentTest->m_details.suiteName, *this);
	}

	if (_lastTest != currentTest)
	{
		_lastTest = currentTest;
		_nextTestBackup = currentTest->m_nextTest;
		onNewIteration(true);
	}
}


Test* const ParameterizedTestAbstract::retrieveCurrentTest()
{
	return retrieveTest(CurrentTest::Details());
}


Test* const ParameterizedTestAbstract::retrieveTest(TestDetails const * const details)
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


bool ParameterizedTestAbstract::hasMoreValues(int advance) const
{
	return (_iteration + advance < (int)valuesSize());
}


void ParameterizedTestAbstract::onNewIteration(bool first)
{
	if (first)
	{
		_iteration = 0;
		if (!hasMoreValues())
		{
			throw runtime_error("No values for parameterized test");
		}
	}
	else
	{
		_iteration++;
	}

	if (hasMoreValues())
	{
		_lastTest->m_nextTest = _testAnchor;
		_testAnchor->m_nextTest = _lastTest;
	}
	else
	{
		_lastTest->m_nextTest = _nextTestBackup;
		_lastTest = nullptr;

		_testAnchor->m_nextTest = _nextTestBackup;
		return;
	}

	peekCurrentValue(_iteration);
}


ParameterizedTestAbstract::TestAnchor::TestAnchor(char const* testName, char const* suiteName, ParameterizedTestAbstract & pt)
	: Test(testName, suiteName),
	_pt(pt)
{
}


void ParameterizedTestAbstract::TestAnchor::RunImpl() const
{
	_pt.onNewIteration(false);
}
