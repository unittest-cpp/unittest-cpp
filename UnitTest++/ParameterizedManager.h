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
		struct IgnoredIndex
		{
			IgnoredIndex(size_t index, bool global) : index(index), global(global) {}
			size_t index;
			bool global;
		};
		
		typedef unordered_map<ParameterizedTestAbstract*, vector<IgnoredIndex>> IgnoredIndexesMap;

		ParameterizedManager();
		virtual ~ParameterizedManager();
		TestListNode* const retrieveTest(TestDetails const * const details);
		void iterate(ParameterizedTestAbstract* const parameterized);
		bool registerParameter(ParameterizedTestAbstract* const parameterized, bool & outFirst);
		vector<IgnoredIndex>::iterator findIgnored(vector<IgnoredIndex> & ignoredIndexes, size_t index);
		void clearNonGlobalIgnoredIndexes();

		TestListNode* _currentTest;
		TestListNode* _nextTestBackup;
		vector<ParameterizedTestAbstract*> _stack;
		IgnoredIndexesMap _ignoredIndexes;
		volatile bool _iterationDone;
		volatile bool _executing;
	};
}

#endif