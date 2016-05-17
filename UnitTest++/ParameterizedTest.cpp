#include "ParameterizedTest.h"

#include "CurrentTest.h"

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
	TestListNode* currentTest = retrieveCurrentTest();

	if (_testAnchor == nullptr)
	{
		_testAnchor = new TestAnchor("ParameterizedTestAnchor", currentTest->m_test->m_details.suiteName, *this);
	}
	if (_testAnchorNode == nullptr)
	{
		_testAnchorNode = new TestListNode(_testAnchor);
	}

	if (_lastTest != currentTest)
	{
		_lastTest = currentTest;
		_nextTestBackup = currentTest->m_next;
		onNewIteration(true);
	}
}


TestListNode* const ParameterizedTestAbstract::retrieveCurrentTest()
{
	return retrieveTest(CurrentTest::Details());
}


TestListNode* const ParameterizedTestAbstract::retrieveTest(TestDetails const * const details)
{
	//TODO This workaround is too far complicated, why not simply add pointer to current test in class CurrentTest ?
	Details2Test::iterator it = _tests.find(details);

	if (it != _tests.end())
	{
		return it->second;
	}

	for (TestListNode* iNode = Test::GetTestList().GetHead(); iNode != nullptr; iNode = iNode->m_next)
	{
		if (&iNode->m_test->m_details == details)
		{
			_tests[details] = iNode;
			return iNode;
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
