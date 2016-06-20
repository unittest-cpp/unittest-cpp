#include "ParameterizedTest.h"

#include <sstream>
#include "ParameterizedManager.h"

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


string ParameterizedTestAbstract::getNameCurrent() const
{
	stringstream output;
	output << (_name.empty() ? "unknown" : _name) << "[" << _iteration << "]";
	return output.str();
}


void ParameterizedTestAbstract::updateIteration()
{
	ParameterizedManager::getInstance().updateParameter(this);
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

	peekCurrentParameter(& ParameterizedManager::getInstance().getCurrentTest()->m_details, _iteration);
}


ParameterizedTestAbstract & ParameterizedTestAbstract::ignoreIndex(size_t index)
{
	ParameterizedManager::getInstance().ignoreIndex(this, index);
	return *this;
}
