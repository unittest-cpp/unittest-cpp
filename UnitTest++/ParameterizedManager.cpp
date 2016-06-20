#include "ParameterizedManager.h"

#include "Test.h"

using namespace std;
using namespace UnitTest;


ParameterizedManager::ParameterizedManager()
	: _currentTest(nullptr),
	_nextTestBackup(nullptr),
	_iterationDone(false),
	_executing(false)
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
	for (TestListNode* iNode = Test::GetTestList().GetHead(); iNode != nullptr; iNode = iNode->m_next)
	{
		// Warning: do not use TestDetails::sameTest here for optimisation reason
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
		if (isCurrentTest(details))
		{
			_executing = true;
		}
		return;
	}
	_currentTest = retrieveTest(details);
	_nextTestBackup = _currentTest->m_next;
	_iterationDone = false;
	_executing = true;
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
		clearNonGlobalIgnoredIndexes();
	}

	_iterationDone = false;
	_executing = false;
}


void ParameterizedManager::clearNonGlobalIgnoredIndexes()
{
	for (IgnoredIndexesMap::iterator it1 = _ignoredIndexes.begin(); it1 != _ignoredIndexes.end();)
	{
		vector<IgnoredIndex> & ignoredIndexes = it1->second;

		for (vector<IgnoredIndex>::iterator it2 = ignoredIndexes.begin(); it2 != ignoredIndexes.end();)
		{
			if (!it2->global)
			{
				it2 = ignoredIndexes.erase(it2);
			}
			else
			{
				it2++;
			}
		}

		if (ignoredIndexes.empty())
		{
			_ignoredIndexes.erase(it1++); // Note: map erase is a bit special
		}
		else
		{
			it1++;
		}
	}
}


void ParameterizedManager::updateParameter(ParameterizedTestAbstract* const parameterized)
{
	vector<IgnoredIndex> & ignoredIndexes = _ignoredIndexes[parameterized];
	bool repeat = true;
	while (repeat)
	{
		iterate(parameterized);

		vector<IgnoredIndex>::iterator ignoredIt = findIgnored(ignoredIndexes, parameterized->_iteration);
		repeat = (ignoredIt != ignoredIndexes.end());
		if (repeat)
		{
			_iterationDone = false;
		}
	}
}


void ParameterizedManager::iterate(ParameterizedTestAbstract* const parameterized)
{
	bool firstIteration = false;
	if (registerParameter(parameterized, firstIteration))
	{
		parameterized->onNewIteration(firstIteration);
	}
}


bool ParameterizedManager::registerParameter(ParameterizedTestAbstract* const parameterized, bool & outFirstIteration)
{
	if (find(_stack.begin(), _stack.end(), parameterized) == _stack.end())
	{
		_iterationDone = true;
		_currentTest->m_next = _currentTest; // Loop itself
		_stack.push_back(parameterized);
		outFirstIteration = true;
		return true;
	}
	if (!_iterationDone)
	{
		if (!_stack.empty() && _stack.back() == parameterized)
		{
			_iterationDone = true;
			outFirstIteration = false;
			return true;
		}
	}
	return false;
}


vector<ParameterizedManager::IgnoredIndex>::iterator ParameterizedManager::findIgnored(vector<IgnoredIndex> & ignoredIndexes, size_t index)
{
	for (vector<IgnoredIndex>::iterator it = ignoredIndexes.begin(); it != ignoredIndexes.end(); it++)
	{
		if (index == it->index)
		{
			return it;
		}
	}
	return ignoredIndexes.end();
}


ParameterizedManager & ParameterizedManager::ignoreIndex(ParameterizedTestAbstract* const parameterized, size_t index)
{
	if (_iterationDone)
	{
		throw runtime_error("can not ignore indexes after iteration began");
	}

	bool global = !_executing;
	vector<IgnoredIndex> & ignoredIndexes = _ignoredIndexes[parameterized];

	if (index >= parameterized->parametersCount())
	{
		throw out_of_range("ignore index is out of range");
	}

	vector<IgnoredIndex>::iterator ignoredIt = findIgnored(ignoredIndexes, index);
	if (ignoredIt != ignoredIndexes.end())
	{
		// Upgrade to global if required
		if (!ignoredIt->global && global)
		{
			ignoredIt->global = global;
		}
		return *this;
	}

	if (ignoredIndexes.size() + 1 == parameterized->parametersCount())
	{
		throw runtime_error("all parameters have been ignored, can not proceed");
	}

	ignoredIndexes.push_back(IgnoredIndex(index, global));

	return *this;
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
