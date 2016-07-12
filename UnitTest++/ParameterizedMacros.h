#ifndef UNITTEST_PARAMETERIZEDMACROS_H
#define UNITTEST_PARAMETERIZEDMACROS_H

#include "TestParameter.h"


#define SET_TEST_PARAMETER_LISTENER(Type, Name, ListenerPtr, SetUpBody) \
	class ParameterizedCreator##Name \
	{ \
	public: \
		ParameterizedCreator##Name() : globalInstance(getGlobalInstance()) {} \
		UnitTest::TestParameter<Type> & globalInstance; \
	private: \
		static UnitTest::TestParameter<Type> & getGlobalInstance() \
		{ \
			static UnitTest::TestParameter<Type> instance(#Name, create(), ListenerPtr); \
			return instance; \
		} \
		static vector<Type> create() { vector<Type> values; SetUpBody return values; } \
	} static parameterizedCreator##Name##Instance; \
	\
	static UnitTest::TestParameter<Type> & Name(parameterizedCreator##Name##Instance.globalInstance)


#define SET_TEST_PARAMETER(Type, Name, SetUpBody) \
	SET_TEST_PARAMETER_LISTENER(Type, Name, nullptr, SetUpBody)


#endif