#ifndef UNITTEST_PARAMETERIZEDMACROS_H
#define UNITTEST_PARAMETERIZEDMACROS_H

#include "ParameterizedTest.h"

// For a local usage, use SET_SUITE_PARAMETERS directly in your "myTestSuite.cpp".
// If you want to reuse a parameterized from an other *.cpp file, use IMPORT_SUITE_PARAMETERS in the new file.
// If you want to generalize a parameterized (example: "myParameterizedGlobals.h"), use SET_SUITE_PARAMETERS in the *.cpp and IMPORT_SUITE_PARAMETERS in the *.h
//
// Warning: please do not use SET_SUITE_PARAMETERS in a *.h file, you might have unexpected duplicate instances !


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
	UnitTest::ParameterizedTest<## Type>  ## IterationName(parameterizedCreator ## IterationName ## Instance.parameters, #IterationName);


#define IMPORT_SUITE_PARAMETERS(Type, IterationName) \
	extern UnitTest::ParameterizedTest<## Type>  ## IterationName;


#endif