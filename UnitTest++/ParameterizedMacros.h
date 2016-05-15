#ifndef UNITTEST_PARAMETERIZEDMACROS_H
#define UNITTEST_PARAMETERIZEDMACROS_H

#include "ParameterizedTest.h"


#define SET_SUITE_PARAMETERS(Type, IterationName, SetUpBody) \
	class ParameterizedCreator ## IterationName\
	{ \
		friend class ParameterizedTest<## Type>; \
	public: \
		ParameterizedCreator ## IterationName() { create(); } \
		vector<## Type> parameters; \
	private: \
		void create(); \
	} parameterizedCreator ## IterationName ## Instance; \
	\
	ParameterizedTest<##Type>  ## IterationName(parameterizedCreator ## IterationName ## Instance.parameters); \
	\
	void ParameterizedCreator ## IterationName::create() \
	## SetUpBody


#endif
