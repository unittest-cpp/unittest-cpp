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
		static Details2Test & getFastSearchMap();
		static Test* const retrieveCurrentTest();
		static Test* const retrieveTest(TestDetails const * const details);
	};
}

#endif