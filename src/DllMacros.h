#ifndef UNITTEST_DLLMACROS_H
#define UNITTEST_DLLMACROS_H

#include "../config.h"

#ifdef UNITTEST_WIN32_DLL
	#define UNITTEST_IMPORT __declspec(dllimport)
	#define UNITTEST_EXPORT	__declspec(dllexport)

	#ifdef UNITTEST_DLL_EXPORT
		#define UNITTEST_LINKAGE UNITTEST_EXPORT
		#define UNITTEST_IMPEXP_TEMPLATE
	#else
		#define UNITTEST_LINKAGE UNITTEST_IMPORT
		#define UNITTEST_IMPEXP_TEMPLATE extern
	#endif

	#define UNITTEST_STDVECTOR_LINKAGE(T) \
		__pragma(warning(push)) \
		__pragma(warning(disable:4231)) \
		UNITTEST_IMPEXP_TEMPLATE template class UNITTEST_LINKAGE std::allocator < T >; \
		UNITTEST_IMPEXP_TEMPLATE template class UNITTEST_LINKAGE std::vector< T >; \
		__pragma(warning(pop))
#else
	#define UNITTEST_IMPORT
	#define UNITTEST_EXPORT
	#define UNITTEST_LINKAGE
	#define UNITTEST_IMPEXP_TEMPLATE
	#define UNITTEST_STDVECTOR_LINKAGE(T)
#endif

#endif
