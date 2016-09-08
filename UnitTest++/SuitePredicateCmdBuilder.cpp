#include "SuitePredicateCmdBuilder.h"

using namespace UnitTest;


SuitePredicateCmdBuilder::SuitePredicateCmdBuilder(int argc, char**argv)
	: _arguments(argc, argv)
{
}


SuitePredicate SuitePredicateCmdBuilder::buildPredicate()
{
	SuitePredicate predicate;

	bool specific = false;
	specific |= readSuiteArgument(_arguments, predicate, "--suite");
	specific |= readTestArgument(_arguments, predicate, "--test");
	specific |= readTestArgument(_arguments, predicate, "");

	if (!specific)
	{
		predicate.addAll();
	}
	return predicate;
}


bool SuitePredicateCmdBuilder::readSuiteArgument(ArgumentsReader & arguments, SuitePredicate & predicate, const string & arg)
{
	int from, count;
	if (!arguments.findArgumentListIndex(arg, from, count))
	{
		return false;
	}
	for (int i = from; i < from + count; i++)
	{
		predicate.addSuite(arguments.getArgument(i).c_str());
	}
	return true;
}


bool SuitePredicateCmdBuilder::readTestArgument(ArgumentsReader & arguments, SuitePredicate & predicate, const string & arg)
{
	int from, count;
	if (!arguments.findArgumentListIndex(arg, from, count))
	{
		return false;
	}
	for (int i = from; i < from + count; i++)
	{
		predicate.addTest(arguments.getArgument(i).c_str());
	}
	return true;
}
