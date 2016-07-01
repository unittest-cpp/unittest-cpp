#ifndef UNITTEST_PARAMETERIZEDMACROS_H
#define UNITTEST_PARAMETERIZEDMACROS_H

#include "ParameterizedTest.h"


#define SET_SUITE_PARAMETERS(Type, Name, SetUpBody) \
	class ParameterizedCreator ## Name \
	{ \
	public: \
		ParameterizedCreator ## Name() : globalInstance(getGlobalInstance()) {} \
		UnitTest::ParameterizedTest<## Type> & globalInstance; \
	private: \
		static UnitTest::ParameterizedTest<## Type> & getGlobalInstance() \
		{ \
			static UnitTest::ParameterizedTest<## Type> instance(#Name, create()); \
			return instance; \
		} \
		static vector<## Type> create() { vector<## Type> parameters; SetUpBody return parameters; } \
	} static parameterizedCreator ## Name ## Instance; \
	\
	static UnitTest::ParameterizedTest<## Type> & ## Name(parameterizedCreator ## Name ## Instance.globalInstance)


#endif