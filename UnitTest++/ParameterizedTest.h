#ifndef UNITTEST_PARAMETERIZEDTEST_H
#define UNITTEST_PARAMETERIZEDTEST_H

#include <string>
#include <map>
#include "Test.h"

namespace UnitTest
{
	using namespace std;

	typedef map<TestDetails const * const, Test*> Details2Test;

	class ParameterizedTest
	{
	public:
		ParameterizedTest();
		virtual ~ParameterizedTest();

	private:
		Test* const retrieveCurrentTest();
		Test* const retrieveTest(TestDetails const * const details);
		Details2Test _tests;
	};
}

#endif