#include "ParameterizedManager.h"

#include "Test.h"
#include "ParameterizedTest.h"

using namespace std;
using namespace UnitTest;


ParameterizedManager::ParameterizedManager()
	: _currentTest(nullptr),
	_nextTestBackup(nullptr)
{
}


ParameterizedManager::~ParameterizedManager()
{
}


ParameterizedManager & ParameterizedManager::getInstance()
{
	static ParameterizedManager stored;
	return stored;
}


TestListNode* const ParameterizedManager::retrieveTest(TestDetails const * const details)
{
	//TODO This workaround is a bit too complicated, why not simply add pointer to current test in class CurrentTest ?
	for (TestListNode* iNode = Test::GetTestList().GetHead(); iNode != nullptr; iNode = iNode->m_next)
	{
		if (&iNode->m_test->m_details == details)
		{
			return iNode;
		}
	}

	throw runtime_error(string("Impossible to retrieve test ") + details->testName);
}


Test* const ParameterizedManager::getCurrentTest() const
{
	return (_currentTest != nullptr) ? _currentTest->m_test : nullptr;
}


bool ParameterizedManager::isCurrentTest(TestDetails const * const details) const
{
	return (_currentTest != nullptr && &_currentTest->m_test->m_details == details);
}


void ParameterizedManager::beginExecute(TestDetails const * const details)
{
	if (_currentTest != nullptr)
	{
		return;
	}
	_currentTest = retrieveTest(details);
	_nextTestBackup = _currentTest->m_next;
	_currentTest->m_next = _currentTest; // Loop itself
}


void ParameterizedManager::endExecute(TestDetails const * const details)
{
	if (!isCurrentTest(details))
	{
		return;
	}

	while (!_stack.empty())
	{
		ParameterizedTestAbstract* iParameterized = _stack.back();
		if (iParameterized->hasMoreParameters(1))
		{
			break;
		}
		else
		{
			_stack.pop_back();
		}
	}

	if (_stack.empty())
	{
		_currentTest->m_next = _nextTestBackup;
		_currentTest = nullptr;
		_nextTestBackup = nullptr;
	}
}


ParameterizedManager::RegisterThen ParameterizedManager::registerParameter(ParameterizedTestAbstract* const parameterized)
{
	if (!_stack.empty() && _stack.back() == parameterized)
	{
		return RegisterThen::ITERATE;
	}
	if (find(_stack.begin(), _stack.end(), parameterized) == _stack.end())
	{
		_stack.push_back(parameterized);
		return RegisterThen::FIRST;
	}
	return RegisterThen::IDLE;
}


const vector<ParameterizedTestAbstract*> & ParameterizedManager::getStack(TestDetails const * const details)
{
	static vector<ParameterizedTestAbstract*> emptyStored;

	if (!isCurrentTest(details))
	{
		return emptyStored;
	}

	return _stack;
}
