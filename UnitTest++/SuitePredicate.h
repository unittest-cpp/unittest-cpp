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

		void addSuite(char const* suiteName);
		void addTest(char const* testName);

		bool operator()(Test const * const test) const;

	private:
		bool mustExecuteSuite(Test const * const test) const;
		bool mustExecuteTest(Test const * const test) const;

		vector<string> _suiteNames;
		vector<string> _testNames;
	};
}

#endif