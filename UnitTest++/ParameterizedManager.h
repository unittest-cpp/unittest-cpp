#ifndef UNITTEST_PARAMETERIZEDMANAGER_H
#define UNITTEST_PARAMETERIZEDMANAGER_H

#include <vector>
#include <map>
#include "TestDetails.h"
#include "TestList.h"

namespace UnitTest
{
	using namespace std;
	class ParameterizedTestAbstract;

	class ParameterizedManager
	{
	public:
		enum RegisterThen
		{
			FIRST,
			ITERATE,
			IDLE
		};

		static ParameterizedManager & getInstance();
		Test* const getCurrentTest() const;
		void beginExecute(TestDetails const * const details);
		void endExecute(TestDetails const * const details);
		RegisterThen registerParameter(ParameterizedTestAbstract* const parameterized);

	private:
		ParameterizedManager();
		virtual ~ParameterizedManager();
		TestListNode* const retrieveTest(TestDetails const * const details);

		TestListNode* _currentTest;
		TestListNode* _nextTestBackup;
		vector<ParameterizedTestAbstract*> _stack;
	};
}

#endif