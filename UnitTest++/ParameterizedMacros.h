#ifndef UNITTEST_PARAMETERIZEDMACROS_H
#define UNITTEST_PARAMETERIZEDMACROS_H

#include "TestParameter.h"


#define SET_SUITE_PARAMETERS(Type, Name, SetUpBody) \
	class ParameterizedCreator##Name \
	{ \
	public: \
		ParameterizedCreator##Name() : globalInstance(getGlobalInstance()) {} \
		UnitTest::TestParameter<Type> & globalInstance; \
	private: \
		static UnitTest::TestParameter<Type> & getGlobalInstance() \
		{ \
			static UnitTest::TestParameter<Type> instance(#Name, create()); \
			return instance; \
		} \
		static vector<Type> create() { vector<Type> parameters; SetUpBody return parameters; } \
	} static parameterizedCreator##Name##Instance; \
	\
	static UnitTest::TestParameter<Type> & Name(parameterizedCreator##Name##Instance.globalInstance)


#endif