cmake_minimum_required(VERSION 2.8.1)
project(UnitTest++)

option(UTPP_USE_PLUS_SIGN "Set this to OFF is you with to use '-cpp' instead of '++' in lib/include paths" ON)

# get the main sources
file(GLOB headers_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} UnitTest++/*.h)
file(GLOB sources_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} UnitTest++/*.cpp)
source_group("" FILES ${headers_} ${sources_})

# get platform specific sources
if (WIN32)
    set(platformDir_ Win32)
else()
    set(platformDir_ Posix)
endif(WIN32)

file(GLOB platformHeaders_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} UnitTest++/${platformDir_}/*.h)
file(GLOB platformSources_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} UnitTest++/${platformDir_}/*.cpp)
source_group(${platformDir_} FILES ${platformHeaders_} ${platformSources_})

# create the lib
add_library(UnitTestPP STATIC ${headers_} ${sources_} ${platformHeaders_} ${platformSources_})

if(${UTPP_USE_PLUS_SIGN})
	set_target_properties(UnitTestPP PROPERTIES OUTPUT_NAME UnitTest++)
endif()


# build the test runner
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp tests/*.h)
source_group( "" FILES ${TEST_SRCS})
add_executable(TestUnitTestPP ${TEST_SRCS})
include_directories(UnitTest++)

if(${UTPP_USE_PLUS_SIGN})
	set_target_properties(TestUnitTestPP PROPERTIES OUTPUT_NAME TestUnitTest++)
endif()

target_link_libraries(TestUnitTestPP UnitTestPP)

# turn on testing
enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -V)

# add the test runner as a test
add_test(NAME TestUnitTestPP COMMAND TestUnitTest++ ${CONFIG_PATH} ${CONFIG_TASKS_PATH} ${SOUND_LOG_PATH})
add_dependencies(check TestUnitTestPP)


# add install targets
# need a custom install path?
# define CMAKE_INSTALL_PREFIX to change root folder
if(${UTPP_USE_PLUS_SIGN})
	set (UTPP_INSTALL_DESTINATION "include/UnitTest++")
else()
	set (UTPP_INSTALL_DESTINATION "include/UnitTestPP")
endif()

install(TARGETS UnitTestPP DESTINATION lib)
install(FILES ${headers_} DESTINATION ${UTPP_INSTALL_DESTINATION})
install(FILES ${platformHeaders_} DESTINATION ${UTPP_INSTALL_DESTINATION}/${platformDir_})