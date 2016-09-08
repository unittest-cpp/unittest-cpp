#ifndef UNITTEST_SUITEPREDICATE_H
#define UNITTEST_SUITEPREDICATE_H

#include <string>
#include <vector>
#include "Test.h"

namespace UnitTest
{
	using namespace std;

	class UNITTEST_LINKAGE SuitePredicate
	{
	public:
		SuitePredicate();
		virtual ~SuitePredicate();

		void addSuite(const string & suiteName);
		void addTest(const string & testName);
		void addSuites(const vector<string> & suiteNames);
		void addTests(const vector<string> & testNames);
		void addAll();
		bool empty();

		bool operator()(Test const * const test) const;

	private:
		bool mustExecuteSuite(Test const * const test) const;
		bool mustExecuteTest(Test const * const test) const;

		vector<string> _suiteNames;
		vector<string> _testNames;
		bool _all;
	};
}

#endif