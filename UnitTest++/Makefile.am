lib_LTLIBRARIES = UnitTest++/libUnitTest++.la
pkgincludedir = $(includedir)

nobase_pkginclude_HEADERS = UnitTest++/AssertException.h UnitTest++/CheckMacros.h UnitTest++/Checks.h UnitTest++/CompositeTestReporter.h UnitTest++/Config.h UnitTest++/CurrentTest.h UnitTest++/DeferredTestReporter.h UnitTest++/DeferredTestResult.h UnitTest++/ExceptionMacros.h UnitTest++/ExecuteTest.h UnitTest++/HelperMacros.h UnitTest++/MemoryOutStream.h UnitTest++/ReportAssert.h UnitTest++/ReportAssertImpl.h UnitTest++/RequireMacros.h UnitTest++/RequiredCheckException.h UnitTest++/RequiredCheckTestReporter.h UnitTest++/Test.h UnitTest++/TestDetails.h UnitTest++/TestList.h UnitTest++/TestMacros.h UnitTest++/TestReporter.h UnitTest++/TestReporterStdout.h UnitTest++/TestResults.h UnitTest++/TestRunner.h UnitTest++/TestSuite.h UnitTest++/ThrowingTestReporter.h UnitTest++/TimeConstraint.h UnitTest++/TimeHelpers.h UnitTest++/UnitTest++.h UnitTest++/UnitTestPP.h UnitTest++/XmlTestReporter.h
UnitTest___libUnitTest___la_SOURCES = UnitTest++/AssertException.cpp UnitTest++/Checks.cpp UnitTest++/CompositeTestReporter.cpp UnitTest++/CurrentTest.cpp UnitTest++/DeferredTestReporter.cpp UnitTest++/DeferredTestResult.cpp UnitTest++/MemoryOutStream.cpp UnitTest++/ReportAssert.cpp UnitTest++/RequiredCheckException.cpp UnitTest++/RequiredCheckTestReporter.cpp UnitTest++/Test.cpp UnitTest++/TestDetails.cpp UnitTest++/TestList.cpp UnitTest++/TestReporter.cpp UnitTest++/TestReporterStdout.cpp UnitTest++/TestResults.cpp UnitTest++/TestRunner.cpp UnitTest++/ThrowingTestReporter.cpp UnitTest++/TimeConstraint.cpp UnitTest++/XmlTestReporter.cpp

if WINDOWS
nobase_pkginclude_HEADERS += UnitTest++/Win32/TimeHelpers.h
UnitTest___libUnitTest___la_SOURCES += UnitTest++/Win32/TimeHelpers.cpp
else
nobase_pkginclude_HEADERS += UnitTest++/Posix/SignalTranslator.h UnitTest++/Posix/TimeHelpers.h
UnitTest___libUnitTest___la_SOURCES += UnitTest++/Posix/SignalTranslator.cpp UnitTest++/Posix/TimeHelpers.cpp
endif

UnitTest___libUnitTest___la_LDFLAGS = -version-number @LIBUNITTEST_SO_VERSION@


