#ifndef UNITTEST_PARAMETERIZEDMACROS_H
#define UNITTEST_PARAMETERIZEDMACROS_H

#include "ParameterizedTest.h"


#define SET_SUITE_PARAMETERS(Type, IterationName, SetUpBody) \
	class ParameterizedCreator ## IterationName \
	{ \
	public: \
		ParameterizedCreator ## IterationName() : globalInstance(getGlobalInstance()) {} \
		UnitTest::ParameterizedTest<## Type> & globalInstance; \
	private: \
		static UnitTest::ParameterizedTest<## Type> & getGlobalInstance() \
		{ \
			static UnitTest::ParameterizedTest<## Type> instance(#IterationName, create()); \
			return instance; \
		} \
		static vector<## Type> create() { vector<## Type> parameters; SetUpBody return parameters; } \
	} static parameterizedCreator ## IterationName ## Instance; \
	\
	static UnitTest::ParameterizedTest<## Type> & ## IterationName(parameterizedCreator ## IterationName ## Instance.globalInstance)


#endif