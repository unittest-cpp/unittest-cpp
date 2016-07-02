#ifndef UNITTEST_PARAMETERIZEDMANAGER_H
#define UNITTEST_PARAMETERIZEDMANAGER_H

#include <vector>
#include <unordered_map>
#include "TestDetails.h"
#include "TestList.h"
#include "TestParameter.h"

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
		void updateParameter(TestParameterAbstract* const parameterized);
		ParameterizedManager & ignoreIndex(TestParameterAbstract* const parameterized, size_t index);
		const vector<TestParameterAbstract*> & getStack(TestDetails const * const details) const;

	private:
		struct IgnoredIndex
		{
			IgnoredIndex(size_t index, bool global) : index(index), global(global) {}
			size_t index;
			bool global;
		};
		
		typedef unordered_map<TestParameterAbstract*, vector<IgnoredIndex>> IgnoredIndexesMap;

		ParameterizedManager();
		virtual ~ParameterizedManager();
		TestListNode* const retrieveTest(TestDetails const * const details);
		void iterate(TestParameterAbstract* const parameterized);
		bool registerParameter(TestParameterAbstract* const parameterized, bool & outFirstIndex);
		bool hasMoreIndexes(TestParameterAbstract* const parameterized);
		vector<IgnoredIndex>::iterator findIgnored(vector<IgnoredIndex> & ignoredIndexes, size_t index);
		void clearNonGlobalIgnoredIndexes();

		TestListNode* _currentTest;
		TestListNode* _nextTestBackup;
		vector<TestParameterAbstract*> _stack;
		IgnoredIndexesMap _ignoredIndexes;
		volatile bool _iterationDone;
		volatile bool _executing;
	};
}

#endif