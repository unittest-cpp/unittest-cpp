unittest_cpp_dir = include_directories('.', '.' / 'Posix')
unittest_cpp_src = files(
    'AssertException.cpp',
    'Checks.cpp',
    'CompositeTestReporter.cpp',
    'CurrentTest.cpp',
    'DeferredTestReporter.cpp',
    'DeferredTestResult.cpp',
    'MemoryOutStream.cpp',
    'ReportAssert.cpp',
    'RequiredCheckException.cpp',
    'RequiredCheckTestReporter.cpp',
    'Test.cpp',
    'TestDetails.cpp',
    'TestList.cpp',
    'TestReporter.cpp',
    'TestReporterStdout.cpp',
    'TestResults.cpp',
    'TestRunner.cpp',
    'ThrowingTestReporter.cpp',
    'TimeConstraint.cpp',
    'XmlTestReporter.cpp'
)
subdir('Posix')

unittest_cpp_lib = static_library('unittest-cpp', 
    sources: unittest_cpp_src,
    include_directories: unittest_cpp_dir)
