#include "ParameterizedManager.h"

#include "Test.h"
#include <sstream>

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


bool ParameterizedManager::isCurrentTest(TestDetails const * const details) const
{
	if (_currentTest == nullptr)
	{
		return false;
	}

	return _currentTest->m_test->m_details.sameTest(*details);
}


bool ParameterizedManager::hasMoreIndexes(TestParameterAbstract* const parameterized)
{
	if (!parameterized->hasMoreValues(1))
	{
		return false;
	}

	vector<IgnoredIndex> & ignoredIndexes = _ignoredIndexes[parameterized];
	if (!ignoredIndexes.empty())
	{
		bool allRemainingsAreIgnored = true;
		for (size_t iIndex = parameterized->_index + 1; iIndex < parameterized->valuesCount(); iIndex++)
		{
			if (findIgnored(ignoredIndexes, iIndex) == ignoredIndexes.end())
			{
				allRemainingsAreIgnored = false;
				break;
			}
		}
		if (allRemainingsAreIgnored)
		{
			return false;
		}
	}

	return true;
}


void ParameterizedManager::beginExecute(TestDetails const * const details)
{
	if (_currentTest != nullptr)
	{
		if (isCurrentTest(details))
		{
			_currentTest->m_next = _nextTestBackup;
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
		TestParameterAbstract* iParameterized = _stack.back();
		if (hasMoreIndexes(iParameterized))
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
		_currentTest = nullptr;
		_nextTestBackup = nullptr;
		clearNonGlobalIgnoredIndexes();
	}
	else
	{
		_currentTest->m_next = _currentTest; // Loop itself
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


void ParameterizedManager::updateParameter(TestParameterAbstract* const parameterized)
{
	importIgnoredParameter(parameterized);
	vector<IgnoredIndex> & ignoredIndexes = _ignoredIndexes[parameterized];
	bool repeat = true;
	while (repeat)
	{
		iterate(parameterized);

		vector<IgnoredIndex>::iterator ignoredIt = findIgnored(ignoredIndexes, parameterized->_index);
		repeat = (ignoredIt != ignoredIndexes.end());
		if (repeat)
		{
			_iterationDone = false;
		}
	}
}


void ParameterizedManager::iterate(TestParameterAbstract* const parameterized)
{
	bool firstIndex = false;
	if (stackParameter(parameterized, firstIndex))
	{
		parameterized->nextIndex(firstIndex);
	}
}


bool ParameterizedManager::stackParameter(TestParameterAbstract* const parameterized, bool & outFirstIndex)
{
	if (find(_stack.begin(), _stack.end(), parameterized) == _stack.end())
	{
		_iterationDone = true;
		outFirstIndex = true;
		_stack.push_back(parameterized);
		return true;
	}
	if (!_iterationDone)
	{
		if (!_stack.empty() && _stack.back() == parameterized)
		{
			_iterationDone = true;
			outFirstIndex = false;
			return true;
		}
	}
	outFirstIndex = false;
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


void ParameterizedManager::importIgnoredParameter(TestParameterAbstract* const parameterized)
{
	IgnoredIndexesByNameMap::iterator it = _pendingIgnoredIndexes.find(parameterized->getName());
	if (it == _pendingIgnoredIndexes.end())
	{
		return;
	}

	vector<IgnoredIndex> & pendingIgnoredIndexes = it->second;
	while (!pendingIgnoredIndexes.empty())
	{
		const IgnoredIndex & iIgnored = pendingIgnoredIndexes.back();
		ignoreIndex(parameterized, iIgnored.index, iIgnored.global ? GLOBAL : LOCAL);
		pendingIgnoredIndexes.pop_back();
	}
}


bool ParameterizedManager::isGlobal(IgnoreScope scope)
{
	switch (scope)
	{
	case AUTO: return !_executing;
	case LOCAL: return false;
	case GLOBAL:return true;
	}
	throw runtime_error("Invalid enum value for ignore scope");
}


ParameterizedManager & ParameterizedManager::ignoreIndex(TestParameterAbstract* const parameterized, size_t index, IgnoreScope scope)
{
	if (_iterationDone)
	{
		throw runtime_error("can not ignore indexes after iteration began");
	}

	bool global = isGlobal(scope);
	vector<IgnoredIndex> & ignoredIndexes = _ignoredIndexes[parameterized];

	if (index >= parameterized->valuesCount())
	{
		return *this;
	}

	vector<IgnoredIndex>::iterator ignoredIt = findIgnored(ignoredIndexes, index);
	if (ignoredIt != ignoredIndexes.end())
	{
		// Upgrade to global if required
		if (global && !ignoredIt->global)
		{
			ignoredIt->global = true;
		}
	}
	else
	{
		if (ignoredIndexes.size() + 1 == parameterized->valuesCount())
		{
			throw runtime_error("all parameters have been ignored, can not proceed");
		}
		ignoredIndexes.push_back(IgnoredIndex(index, global));
	}
	return *this;
}


ParameterizedManager & ParameterizedManager::ignoreIndex(const string & parameterName, size_t index, IgnoreScope scope)
{
	if (parameterName.empty())
	{
		throw invalid_argument("Empty parameter name given in ignore index");
	}

	// Note: do not care about duplicates indexes for now
	bool global = isGlobal(scope);
	_pendingIgnoredIndexes[parameterName].push_back(IgnoredIndex(index, global));

	return *this;
}


ParameterizedManager & ParameterizedManager::ignoreIndexes(const string & parametersArrayRange, IgnoreScope scope)
{
	string parameterName;
	vector<int> indexes = parseParameter(parametersArrayRange, parameterName);
	for (size_t i = 0; i < indexes.size(); i++)
	{
		ignoreIndex(parameterName, indexes[i], scope);
	}
	return *this;
}


vector<int> ParameterizedManager::parseParameter(const string & parametersArrayRange, string & outParameterName)
{
	if (parametersArrayRange.empty())
	{
		throw invalid_argument("Empty parameter array range given");
	}

	size_t openPos = parametersArrayRange.find('[');
	size_t closePos = parametersArrayRange.find(']');
	if (openPos == 0 || openPos == string::npos || closePos == string::npos || closePos < openPos)
	{
		throw invalid_argument("Misformatted parameter array range index, should be pzMyParam[0,1,2]");
	}

	outParameterName = parametersArrayRange.substr(0, openPos);
	string indexes = parametersArrayRange.substr(openPos + 1, closePos - openPos - 1);

	stringstream stream(indexes);
	string item;
	vector<int> indexesList;
	while (getline(stream, item, ','))
	{
		int index = stoi(item);
		indexesList.push_back(index);
	}

	return indexesList;
}


bool ParameterizedManager::isIndexIgnored(TestParameterAbstract* const parameterized, size_t index)
{
	vector<IgnoredIndex> & ignoredIndexes = _ignoredIndexes[parameterized];
	vector<IgnoredIndex>::iterator ignoredIt = findIgnored(ignoredIndexes, index);
	return (ignoredIt != ignoredIndexes.end());
}


const vector<TestParameterAbstract*> & ParameterizedManager::getStack(TestDetails const * const details) const
{
	if (!isCurrentTest(details))
	{
		static vector<TestParameterAbstract*> emptyStored;
		return emptyStored;
	}
	return _stack;
}
