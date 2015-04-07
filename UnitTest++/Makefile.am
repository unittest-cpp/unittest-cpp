lib_LTLIBRARIES = UnitTest++/libUnitTest++.la
pkgincludedir = $(includedir)/UnitTest++/
nobase_pkginclude_HEADERS = UnitTest++/UnitTest++.h UnitTest++/UnitTestPP.h UnitTest++/Config.h UnitTest++/HelperMacros.h UnitTest++/Test.h UnitTest++/TestDetails.h UnitTest++/TestList.h UnitTest++/TestSuite.h UnitTest++/TestResults.h UnitTest++/TestMacros.h UnitTest++/CheckMacros.h UnitTest++/TestRunner.h UnitTest++/TimeConstraint.h UnitTest++/ExecuteTest.h UnitTest++/AssertException.h UnitTest++/MemoryOutStream.h UnitTest++/CurrentTest.h  UnitTest++/Checks.h UnitTest++/TimeHelpers.h  UnitTest++/ExceptionMacros.h UnitTest++/ReportAssert.h UnitTest++/ReportAssertImpl.h UnitTest++/TestReporter.h UnitTest++/TestReporterStdout.h UnitTest++/CompositeTestReporter.h UnitTest++/DeferredTestReporter.h UnitTest++/DeferredTestResult.h
UnitTest___libUnitTest___la_SOURCES = UnitTest++/AssertException.cpp UnitTest++/Test.cpp UnitTest++/Checks.cpp UnitTest++/TestRunner.cpp UnitTest++/TestResults.cpp UnitTest++/TestReporter.cpp UnitTest++/TestReporterStdout.cpp UnitTest++/ReportAssert.cpp UnitTest++/TestList.cpp UnitTest++/TimeConstraint.cpp UnitTest++/TestDetails.cpp UnitTest++/MemoryOutStream.cpp UnitTest++/DeferredTestReporter.cpp UnitTest++/DeferredTestResult.cpp UnitTest++/XmlTestReporter.cpp UnitTest++/CurrentTest.cpp  UnitTest++/CompositeTestReporter.cpp

if WINDOWS 
nobase_pkginclude_HEADERS += UnitTest++/Win32/TimeHelpers.h
UnitTest___libUnitTest___la_SOURCES += UnitTest++/Win32/TimeHelpers.cpp
else 
nobase_pkginclude_HEADERS += UnitTest++/Posix/SignalTranslator.h UnitTest++/Posix/TimeHelpers.h
UnitTest___libUnitTest___la_SOURCES += UnitTest++/Posix/SignalTranslator.cpp UnitTest++/Posix/TimeHelpers.cpp
endif

UnitTest___libUnitTest___la_LDFLAGS = -version-number @LIBUNITTEST_SO_VERSION@


