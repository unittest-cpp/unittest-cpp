#include "ParameterizedSuite.h"

#include <string>
#include <iostream>
#include "TestList.h"

using namespace std;
using namespace UnitTest;


ParameterizedSuiteAbstract::ParameterizedSuiteAbstract(const string & suiteName)
	: _iterationBranched(false),
	_iteration(0),
	_suiteName(suiteName),
	_testName(suiteName + "ParameterizedSuite_anchor"),
	_firstOuterTest(nullptr),
	_lastOfSuite(nullptr),
	_testAnchor(nullptr) // Important, even if defined just above (please read comment)
{
	// WARNING: this is pointer because of memory problems with suiteName/testName.c_str(),
	// the constructor does not initialize them in the right order
	_testAnchor = new TestAnchor(*this);
}


ParameterizedSuiteAbstract::~ParameterizedSuiteAbstract()
{
	if (_testAnchor != nullptr)
	{
		delete _testAnchor;
		_testAnchor = nullptr;
	}
}


size_t ParameterizedSuiteAbstract::getIteration() const
{
	return _iteration;
}


void ParameterizedSuiteAbstract::ensureInitialized()
{
	tryPeekFirstValue();
	tryBranchUniqueTest();
}


bool ParameterizedSuiteAbstract::hasMoreValues(int advance) const
{
	return (_iteration + advance < (int)valuesSize());
}


bool ParameterizedSuiteAbstract::tryPeekFirstValue()
{
	if (_iterationBranched)
	{
		return false;
	}

	peekCurrentValue();
	return false;
}


bool ParameterizedSuiteAbstract::tryBranchUniqueTest()
{
	if (_iterationBranched)
	{
		return false;
	}

	//TODO find a way to allow single parameterized test execution
	static bool messageDisplayed = false;
	if (!messageDisplayed)
	{
		messageDisplayed = true;
		cout << "WARNING: test of parameterized suite " << _suiteName
			<< " can not be executed alone, test will be executed with first value only." << endl;
	}

	return true;
}


bool ParameterizedSuiteAbstract::branchTestsForIteration()
{
	if (_iterationBranched)
	{
		return false;
	}

	for (Test* iTest = _testAnchor; iTest != nullptr; iTest = iTest->m_nextTest)
	{
		bool inSameSuite = (strcmp(iTest->m_details.suiteName, _suiteName.c_str()) == 0);
		bool ownAnchor = (iTest == _testAnchor);
		bool isOtherParameterizedSuite = (iTest != _testAnchor && dynamic_cast<TestAnchor*>(iTest) != nullptr);
		if (!inSameSuite || isOtherParameterizedSuite)
		{
			_firstOuterTest = iTest;
			break;
		}
		_lastOfSuite = iTest;
	}

	_lastOfSuite->m_nextTest = _testAnchor;

	_iterationBranched = true;
	return true;
}


bool ParameterizedSuiteAbstract::unbranchIterationIfLast()
{
	if (!_iterationBranched)
	{
		return false;
	}

	if (hasMoreValues(1))
	{
		return false;
	}

	_lastOfSuite->m_nextTest = _firstOuterTest;
	return true;
}


void ParameterizedSuiteAbstract::onNewIteration()
{
	if (_iterationBranched)
	{
		_iteration++;
	}

	bool justBranched = branchTestsForIteration();
	bool justUnbranched = unbranchIterationIfLast();

	if (justBranched && justUnbranched)
	{
		// If no values to test, skip all tests
		if (valuesSize() == 0)
		{
			_testAnchor->m_nextTest = _firstOuterTest;
			return;
		}
	}

	peekCurrentValue();
}


ParameterizedSuiteAbstract::TestAnchor::TestAnchor(ParameterizedSuiteAbstract & parameterized)
	: Test(parameterized._testName.c_str(), parameterized._suiteName.c_str()),
	_parameterized(parameterized)
{
	Test::GetTestList().Add(this);
}


void ParameterizedSuiteAbstract::TestAnchor::RunImpl() const
{
	_parameterized.onNewIteration();
}
