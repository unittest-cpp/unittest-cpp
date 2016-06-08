#include "ParameterizedTest.h"


using namespace std;
using namespace UnitTest;


ParameterizedTestAbstract::ParameterizedTestAbstract(const string & name)
	: _name(name),
	_iteration(0)
{
}


ParameterizedTestAbstract::~ParameterizedTestAbstract()
{
}


size_t ParameterizedTestAbstract::getIteration()
{
	updateIteration();
	return _iteration;
}


void ParameterizedTestAbstract::updateIteration()
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
	if (!first && !hasMoreParameters(1))
	{
		first = true;
	}

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
		_iteration++;
	}

	peekCurrentParameter(_iteration);
}
