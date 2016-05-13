#ifndef UNITTEST_PARAMETERIZEDMACROS_H
#define UNITTEST_PARAMETERIZEDMACROS_H

#include "ParameterizedSuite.h"


#define SET_SUITE_PARAMETERS(Type, IterationName, SetUpBody) \
	class ParameterizedCreator ## IterationName\
	{ \
		friend class ParameterizedSuite<## Type>; \
	public: \
		ParameterizedCreator ## IterationName() { create(); } \
		vector<## Type> parameters; \
	private: \
		void create(); \
	} parameterizedCreator ## IterationName ## Instance; \
	\
	ParameterizedSuite<##Type>  ## IterationName(UnitTestSuite::GetSuiteName(), parameterizedCreator ## IterationName ## Instance.parameters); \
	\
	void ParameterizedCreator ## IterationName::create() \
	## SetUpBody


#define PARAMETERIZED_SUITE(Name, Type, IterationName, SetUpBody) \
	SUITE(## Name) \
	{ \
		SET_SUITE_PARAMETERS(## Type, ## IterationName, ## SetUpBody) \
	} \
	namespace Suite ## Name

#endif
