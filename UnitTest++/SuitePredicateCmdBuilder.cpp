#include "SuitePredicateCmdBuilder.h"

using namespace UnitTest;


SuitePredicateCmdBuilder::SuitePredicateCmdBuilder(int argc, char**argv)
	: _arguments(argc, argv)
{
}


SuitePredicate SuitePredicateCmdBuilder::buildPredicate(bool allowImplicitTestArg)
{
	SuitePredicate predicate;

	predicate.addSuites(_arguments.extractValues("--suite"));
	predicate.addTests(_arguments.extractValues("--test"));
	predicate.addTests(_arguments.extractValues(""));

	if (predicate.empty())
	{
		predicate.addAll();
	}

	return predicate;
}

