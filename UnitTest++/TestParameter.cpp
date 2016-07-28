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


bool TestParameterAbstract::hasMoreValues(int advance) const
{
	return (_index + advance < (int)valuesCount());
}


void TestParameterAbstract::nextIndex(bool first)
{
	if (first)
	{
		if (valuesCount() == 0)
		{
			throw runtime_error("No values for parameterized test");
		}
		_index = 0;
	}
	else
	{
		_index++;
	}

	peekCurrent(&ParameterizedManager::getInstance().getCurrentTest()->m_test->m_details, _index);
}


TestParameterAbstract & TestParameterAbstract::ignoreIndex(size_t index, IgnoreScope scope)
{
	ParameterizedManager::getInstance().ignoreIndex(this, index, scope);
	return *this;
}


bool TestParameterAbstract::isIndexIgnored(size_t index)
{
	return ParameterizedManager::getInstance().isIndexIgnored(this, index);
}
