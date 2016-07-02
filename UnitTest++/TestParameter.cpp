#include "TestParameter.h"

#include <sstream>
#include "ParameterizedManager.h"

using namespace std;
using namespace UnitTest;


TestParameterAbstract::TestParameterAbstract(const string & name)
	: _name(name),
	_index(0)
{
}


TestParameterAbstract::~TestParameterAbstract()
{
}


size_t TestParameterAbstract::getCurrentIndex()
{
	updateCurrentIndex();
	return _index;
}


string TestParameterAbstract::getNameCurrent() const
{
	stringstream output;
	output << (_name.empty() ? "unknown" : _name) << "[" << _index << "]";
	return output.str();
}


void TestParameterAbstract::updateCurrentIndex()
{
	ParameterizedManager::getInstance().updateParameter(this);
}


bool TestParameterAbstract::hasMoreParameters(int advance) const
{
	return (_index + advance < (int)parametersCount());
}


void TestParameterAbstract::nextIndex(bool first)
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


TestParameterAbstract & TestParameterAbstract::ignoreIndex(size_t index)
{
	ParameterizedManager::getInstance().ignoreIndex(this, index);
	return *this;
}
