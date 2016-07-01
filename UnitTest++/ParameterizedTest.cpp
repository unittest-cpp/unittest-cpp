#include "ParameterizedTest.h"

#include <sstream>
#include "ParameterizedManager.h"

using namespace std;
using namespace UnitTest;


ParameterizedTestAbstract::ParameterizedTestAbstract(const string & name)
	: _name(name),
	_index(0)
{
}


ParameterizedTestAbstract::~ParameterizedTestAbstract()
{
}


size_t ParameterizedTestAbstract::getCurrentIndex()
{
	updateCurrentIndex();
	return _index;
}


string ParameterizedTestAbstract::getNameCurrent() const
{
	stringstream output;
	output << (_name.empty() ? "unknown" : _name) << "[" << _index << "]";
	return output.str();
}


void ParameterizedTestAbstract::updateCurrentIndex()
{
	ParameterizedManager::getInstance().updateParameter(this);
}


bool ParameterizedTestAbstract::hasMoreParameters(int advance) const
{
	return (_index + advance < (int)parametersCount());
}


void ParameterizedTestAbstract::nextIndex(bool first)
{
	if (first)
	{
		if (parametersCount() == 0)
		{
			throw runtime_error("No values for parameterized test");
		}
		_index = 0;
	}
	else
	{
		_index++;
	}

	peekCurrentParameter(&ParameterizedManager::getInstance().getCurrentTest()->m_details, _index);
}


ParameterizedTestAbstract & ParameterizedTestAbstract::ignoreIndex(size_t index)
{
	ParameterizedManager::getInstance().ignoreIndex(this, index);
	return *this;
}
