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
	_excludedIndexes.clear();
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


void ParameterizedManager::updateParameter(ParameterizedTestAbstract* const parameterized)
{
	const vector<size_t> & excludedIndexes = _excludedIndexes[parameterized];
	bool repeat = true;
	while (repeat)
	{
		iterate(parameterized);

		repeat = (find(excludedIndexes.begin(), excludedIndexes.end(), parameterized->_iteration) != excludedIndexes.end());
		if (repeat)
		{
			_iterationDone = false;
		}
	}
}


void ParameterizedManager::iterate(ParameterizedTestAbstract* const parameterized)
{
	RegisterThen then = registerParameter(parameterized);
	if (then == ParameterizedManager::FIRST)
	{
		parameterized->onNewIteration(true);
	}
	else if (then == ParameterizedManager::ITERATE)
	{
		parameterized->onNewIteration(false);
	}
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


void ParameterizedManager::excludeIndex(ParameterizedTestAbstract* const parameterized, size_t index)
{
	if (_iterationDone)
	{
		throw runtime_error("can not exclude indexes after iteration began");
	}

	vector<size_t> & excludedIndexes = _excludedIndexes[parameterized];

	if (index >= parameterized->parametersCount())
	{
		throw out_of_range("excluded index is out of range");
	}

	if (find(excludedIndexes.begin(), excludedIndexes.end(), index) != excludedIndexes.end())
	{
		return; // already inserted
	}

	if (excludedIndexes.size() + 1 == parameterized->parametersCount())
	{
		throw runtime_error("all parameters have been excluded, can not proceed");
	}

	excludedIndexes.push_back(index);
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
