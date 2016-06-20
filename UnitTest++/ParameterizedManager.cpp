#include "ParameterizedManager.h"

#include "Test.h"

using namespace std;
using namespace UnitTest;


ParameterizedManager::ParameterizedManager()
	: _currentTest(nullptr),
	_nextTestBackup(nullptr),
	_iterationDone(false)
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
		// Note: do not use TestDetails::sameTest here for optimisation reason
		if (&iNode->m_test->m_details == details)
		{
			return iNode;
		}
	}

	string errorMessage = "Impossible to retrieve test ";
	errorMessage += details->testName;
	throw runtime_error(errorMessage);
}


Test* const ParameterizedManager::getCurrentTest() const
{
	if (_currentTest == nullptr)
	{
		return nullptr;
	}

	return _currentTest->m_test;
}


bool ParameterizedManager::isCurrentTest(TestDetails const * const details) const
{
	if (_currentTest == nullptr)
	{
		return false;
	}

	return _currentTest->m_test->m_details.sameTest(*details);
}


void ParameterizedManager::beginExecute(TestDetails const * const details)
{
	if (_currentTest != nullptr)
	{
		return;
	}
	_currentTest = retrieveTest(details);
	_nextTestBackup = _currentTest->m_next;
	_iterationDone = false;
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

	_iterationDone = false;
}


ParameterizedManager::RegisterThen ParameterizedManager::registerParameter(ParameterizedTestAbstract* const parameterized)
{
	if (find(_stack.begin(), _stack.end(), parameterized) == _stack.end())
	{
		_iterationDone = true;
		_currentTest->m_next = _currentTest; // Loop itself
		_stack.push_back(parameterized);
		return RegisterThen::FIRST;
	}
	if (!_iterationDone)
	{
		if (!_stack.empty() && _stack.back() == parameterized)
		{
			_iterationDone = true;
			return RegisterThen::ITERATE;
		}
	}
	return RegisterThen::IDLE;
}


const vector<ParameterizedTestAbstract*> & ParameterizedManager::getStack(TestDetails const * const details) const
{
	if (!isCurrentTest(details))
	{
		static vector<ParameterizedTestAbstract*> emptyStored;
		return emptyStored;
	}
	return _stack;
}
