#include "SuitePredicateCmdBuilder.h"

using namespace UnitTest;


SuitePredicateCmdBuilder::SuitePredicateCmdBuilder(int argc, char**argv)
	: _arguments(argc, argv)
{
}


SuitePredicate SuitePredicateCmdBuilder::buildPredicate(bool allowImplicitArgs)
{
	SuitePredicate predicate;

	predicate.addSuites(_arguments.extractValues("--suite"));
	predicate.addTests(_arguments.extractValues("--test"));

	if (allowImplicitArgs)
	{
		vector<string> implicitArguments = _arguments.extractValues("");
		predicate.addSuites(implicitArguments);
		predicate.addTests(implicitArguments);
	}

	if (predicate.empty())
	{
		predicate.addAll();
	}

	return predicate;
}

