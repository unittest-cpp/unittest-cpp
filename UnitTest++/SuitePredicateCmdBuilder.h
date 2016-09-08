#ifndef UNITTEST_TESTRUNNERCMD_H
#define UNITTEST_TESTRUNNERCMD_H

#include "ArgumentsReader.h"
#include "SuitePredicate.h"

namespace UnitTest {

	class SuitePredicateCmdBuilder
	{
	public:
		SuitePredicateCmdBuilder(int argc, char**argv);
		SuitePredicate buildPredicate();

	protected:
		bool readSuiteArgument(ArgumentsReader & arguments, SuitePredicate & predicate, const string & arg);
		bool readTestArgument(ArgumentsReader & arguments, SuitePredicate & predicate, const string & arg);

	private:
		ArgumentsReader _arguments;
	};
}

#endif