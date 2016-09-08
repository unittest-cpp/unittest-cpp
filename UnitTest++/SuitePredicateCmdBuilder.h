#ifndef UNITTEST_TESTRUNNERCMD_H
#define UNITTEST_TESTRUNNERCMD_H

#include "ArgumentsReader.h"
#include "SuitePredicate.h"

namespace UnitTest {

	class SuitePredicateCmdBuilder
	{
	public:
		SuitePredicateCmdBuilder(int argc, char**argv);
		SuitePredicate buildPredicate(bool allowImplicitArgs = true);

	private:
		ArgumentsReader _arguments;
	};
}

#endif