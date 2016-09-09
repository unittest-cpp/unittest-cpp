#include "PredicateCmdBuilder.h"

using namespace UnitTest;


void PredicateCmdBuilder::fillSuitePredicate(const ArgumentsReader & arguments, SuitePredicate & outPredicate, bool allowImplicitArgs)
{
	outPredicate.addSuites(arguments.extractValues("--suite"));
	outPredicate.addTests(arguments.extractValues("--test"));

	if (allowImplicitArgs)
	{
		vector<string> implicitArguments = arguments.extractValues("");
		outPredicate.addSuites(implicitArguments);
		outPredicate.addTests(implicitArguments);
	}

	if (outPredicate.empty())
	{
		outPredicate.addAll();
	}
}

