#ifndef UNITTEST_PARAMETERIZEDMACROS_H
#define UNITTEST_PARAMETERIZEDMACROS_H

#include "ParameterizedSuite.h"


#define SET_SUITE_PARAMETERS(Name, Type, SetUpBody) \
	class ParameterizedCreator ## Name\
	{ \
		friend class ParameterizedSuite<## Type>; \
	public: \
		ParameterizedCreator ## Name() { create(); } \
		vector<## Type> parameters; \
	private: \
		void create(); \
	} parameterizedCreator ## Name ## Instance; \
	\
	ParameterizedSuite<##Type>  ## Name(UnitTestSuite::GetSuiteName(), parameterizedCreator ## Name ## Instance.parameters); \
	\
	void ParameterizedCreator ## Name::create() \
	## SetUpBody

#endif
