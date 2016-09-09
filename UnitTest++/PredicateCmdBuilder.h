#ifndef UNITTEST_PredicateCmdBuilder_H
#define UNITTEST_TESTRUNNERCMD_H

#include "ArgumentsReader.h"
#include "SuitePredicate.h"

namespace UnitTest {

	class PredicateCmdBuilder
	{
	public:
		static void fillSuitePredicate(const ArgumentsReader & arguments, SuitePredicate & outPredicate, bool allowImplicitArgs = true);
	};
}

#endif