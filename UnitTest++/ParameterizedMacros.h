#ifndef UNITTEST_PARAMETERIZEDMACROS_H
#define UNITTEST_PARAMETERIZEDMACROS_H

#include "ParameterizedTest.h"


#define SET_SUITE_PARAMETERS(Type, IterationName, SetUpBody) \
	class ParameterizedCreator ## IterationName\
	{ \
	public: \
		ParameterizedCreator ## IterationName() { create(); } \
		vector<## Type> parameters; \
	private: \
		void create() \
		## SetUpBody \
	} parameterizedCreator ## IterationName ## Instance; \
	\
	UnitTest::ParameterizedTest<## Type>  ## IterationName(parameterizedCreator ## IterationName ## Instance.parameters);


#endif