#ifndef UNITTEST_PARAMETERIZEDMANAGER_H
#define UNITTEST_PARAMETERIZEDMANAGER_H

#include <map>
#include "TestDetails.h"
#include "TestList.h"

namespace UnitTest
{
	using namespace std;

	typedef map<TestDetails const * const, TestListNode*> Details2Test;

	class ParameterizedManager
	{
	public:
		static ParameterizedManager & getInstance();
		TestListNode* const retrieveTest(TestDetails const * const details);

	private:
		ParameterizedManager();
		virtual ~ParameterizedManager();

		Details2Test _tests;
	};
}

#endif