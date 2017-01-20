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
		TestListNode* const getCurrentTest() const { return _currentTest; }
		bool isCurrentTest(TestDetails const * const details) const;
		void beginExecute(TestDetails const * const details);
		void endExecute(TestDetails const * const details);
		void updateParameter(TestParameterAbstract* const parameterized);
		ParameterizedManager & ignoreIndex(TestParameterAbstract* const parameterized, size_t index, IgnoreScope scope = AUTO);
		ParameterizedManager & ignoreIndex(const string & parameterName, size_t index, IgnoreScope scope = AUTO);
		ParameterizedManager & ignoreIndexes(const string & parametersArrayRange, IgnoreScope scope = AUTO);
		bool isIndexIgnored(TestParameterAbstract* const parameterized, size_t index);
		const vector<TestParameterAbstract*> & getStack(TestDetails const * const details) const;

	private:
		struct IgnoredIndex
		{
			IgnoredIndex(size_t index, bool global) : index(index), global(global) {}
			size_t index;
			bool global;
		};

		typedef unordered_map<TestParameterAbstract*, vector<IgnoredIndex>> IgnoredIndexesMap;
		typedef unordered_map<string, vector<IgnoredIndex>> IgnoredIndexesByNameMap;

		ParameterizedManager();
		virtual ~ParameterizedManager();
		TestListNode* const retrieveTest(TestDetails const * const details);
		void iterate(TestParameterAbstract* const parameterized);
		bool stackParameter(TestParameterAbstract* const parameterized, bool & outFirstIndex);
		bool hasMoreIndexes(TestParameterAbstract* const parameterized);
		bool isGlobal(IgnoreScope scope);
		vector<IgnoredIndex>::iterator findIgnored(vector<IgnoredIndex> & ignoredIndexes, size_t index);
		void importIgnoredParameter(TestParameterAbstract* const parameterized);
		void clearNonGlobalIgnoredIndexes();

		static vector<int> parseParameter(const string & parametersArrayRange, string & outParameterName);

		TestListNode* _currentTest;
		TestListNode* _nextTestBackup;
		vector<TestParameterAbstract*> _stack;
		IgnoredIndexesMap _ignoredIndexes;
		IgnoredIndexesByNameMap _pendingIgnoredIndexes;
		volatile bool _iterationDone;
		volatile bool _executing;
	};
}

#endif