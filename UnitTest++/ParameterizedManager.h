#ifndef UNITTEST_PARAMETERIZEDMANAGER_H
#define UNITTEST_PARAMETERIZEDMANAGER_H

#include <vector>
#include <unordered_map>
#include "TestDetails.h"
#include "TestList.h"
#include "ParameterizedTest.h"

namespace UnitTest
{
	using namespace std;

	class ParameterizedManager
	{
	public:
		static ParameterizedManager & getInstance();
		Test* const getCurrentTest() const;
		bool isCurrentTest(TestDetails const * const details) const;
		void beginExecute(TestDetails const * const details);
		void endExecute(TestDetails const * const details);
		void updateParameter(ParameterizedTestAbstract* const parameterized);
		ParameterizedManager & ignoreIndex(ParameterizedTestAbstract* const parameterized, size_t index);
		const vector<ParameterizedTestAbstract*> & getStack(TestDetails const * const details) const;

	private:
		typedef unordered_map<ParameterizedTestAbstract*, vector<size_t>> ParameterizedIndexes;

		ParameterizedManager();
		virtual ~ParameterizedManager();
		TestListNode* const retrieveTest(TestDetails const * const details);
		void dumpGlobalIgnoredIndexes(ParameterizedTestAbstract* const parameterized);
		void iterate(ParameterizedTestAbstract* const parameterized);
		bool registerParameter(ParameterizedTestAbstract* const parameterized, bool & outFirst);

		TestListNode* _currentTest;
		TestListNode* _nextTestBackup;
		vector<ParameterizedTestAbstract*> _stack;
		ParameterizedIndexes _ignoredIndexes;
		ParameterizedIndexes _ignoredIndexesGlobal;
		volatile bool _iterationDone;
		volatile bool _executing;
	};
}

#endif