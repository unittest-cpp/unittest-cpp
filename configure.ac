#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([UnitTest++],
        m4_esyscmd_s([git describe --tags | cut -c2-]),
        [pjohnmeyer@gmail.com],
        [unittest-cpp])

AC_CONFIG_SRCDIR([UnitTest++/TestDetails.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([UnitTest++.pc])

AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_SILENT_RULES([yes])

AC_CANONICAL_HOST

dnl Detect Windows, as it doesn't implement UNIX signals and requires special code
AM_CONDITIONAL([WINDOWS],
               [test "${host//mingw/}" != "${host}" -o "${host//msvc/}" != "${host}"])

LT_INIT()

AC_SUBST([LIBUNITTEST_SO_VERSION], [2:0:0])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AX_CXX_COMPILE_STDCXX_11(noext, optional)

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([sys/time.h unistd.h setjmp.h signal.h cassert cstddef cstdio cstring exception iosfwd iostream sstream string vector])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday strstr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
