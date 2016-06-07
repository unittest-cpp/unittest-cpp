#include "ParameterizedTest.h"

#include "CurrentTest.h"
#include "ParameterizedManager.h"

using namespace std;
using namespace UnitTest;


ParameterizedTestAbstract::ParameterizedTestAbstract()
	: _iteration(0),
	_testAnchor(nullptr),
	_testAnchorNode(nullptr),
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
	if (_testAnchorNode != nullptr)
	{
		delete _testAnchorNode;
		_testAnchorNode = nullptr;
	}
}


size_t ParameterizedTestAbstract::getIteration()
{
	ensureInitialized();
	return _iteration;
}


Test* const ParameterizedTestAbstract::getLastTest() const
{
	return _lastTest->m_test;
}


void ParameterizedTestAbstract::ensureInitialized()
{
	TestListNode* currentTest = ParameterizedManager::getInstance().retrieveTest(CurrentTest::Details());

	if (_lastTest != currentTest)
	{
		_lastTest = currentTest;
		_nextTestBackup = currentTest->m_next;
		onNewIteration(true);
	}
}


bool ParameterizedTestAbstract::hasMoreParameters(int advance) const
{
	return (_iteration + advance < (int)parametersCount());
}


void ParameterizedTestAbstract::onNewIteration(bool first)
{
	if (first)
	{
		if (parametersCount() == 0)
		{
			throw runtime_error("No values for parameterized test");
		}
		_iteration = 0;
		newAnchor();
	}
	else
	{
		_iteration++;
	}

	if (hasMoreParameters())
	{
		_lastTest->m_next = _testAnchorNode;
		_testAnchorNode->m_next = _lastTest;
	}
	else
	{
		_lastTest->m_next= _nextTestBackup;
		_lastTest = nullptr;

		_testAnchorNode->m_next = _nextTestBackup;
		return;
	}

	peekCurrentParameter(_iteration);
}


void ParameterizedTestAbstract::newAnchor()
{
	if (_testAnchor != nullptr)
	{
		delete _testAnchor;
	}
	if (_testAnchorNode != nullptr)
	{
		delete _testAnchorNode;
	}

	// Important: create anchor with exactly same details than original test for ensure it will pass all test filters the same way as original
	_testAnchor = new TestAnchor(_lastTest->m_test->m_details.testName, _lastTest->m_test->m_details.suiteName, *this);
	_testAnchorNode = new TestListNode(_testAnchor);
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
