#include "ParameterizedTest.h"

#include "CurrentTest.h"
#include "ParameterizedManager.h"

using namespace std;
using namespace UnitTest;


ParameterizedTestAbstract::ParameterizedTestAbstract()
	: _iteration(0)
{
}


ParameterizedTestAbstract::~ParameterizedTestAbstract()
{
}


size_t ParameterizedTestAbstract::getIteration()
{
	ensureInitialized();
	return _iteration;
}


Test* const ParameterizedTestAbstract::getLastTest() const
{
	return ParameterizedManager::getInstance().getCurrentTest();
}


void ParameterizedTestAbstract::ensureInitialized()
{
	ParameterizedManager::RegisterThen then = ParameterizedManager::getInstance().registerParameter(this);
	if (then == ParameterizedManager::FIRST)
	{
		onNewIteration(true);
	}
	else if (then == ParameterizedManager::ITERATE)
	{
		onNewIteration(false);
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
	}
	else
	{
		_iteration = (hasMoreParameters(1)) ? _iteration + 1 : 0;
	}

	peekCurrentParameter(_iteration);
}
