cmake_minimum_required(VERSION 2.8.1)
project(UnitTest++)

option(UTPP_USE_PLUS_SIGN "Set this to OFF is you with to use '-cpp' instead of '++' in lib/include paths" ON)

# get the main sources
file(GLOB headers_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.h)
file(GLOB sources_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
source_group("" FILES ${headers_} ${sources_})

# get platform specific sources
if (WIN32)
    set(PLAT_DIR Win32)
else()
    set(PLAT_DIR Posix)
endif(WIN32)

file(GLOB PLAT_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/${PLAT_DIR}/*.cpp src/${PLAT_DIR}/*.h)
source_group(${PLAT_DIR} FILES ${PLAT_SRCS})

# create the lib
add_library(UnitTestPP STATIC ${headers_} ${sources_} ${PLAT_SRCS})

if(${UTPP_USE_PLUS_SIGN})
	set_target_properties(UnitTestPP PROPERTIES OUTPUT_NAME UnitTest++)
endif()

include_directories(src)

# build the test runner
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/tests/*.cpp src/tests/*.h)
source_group( "" FILES ${TEST_SRCS})
add_executable(TestUnitTestPP ${TEST_SRCS})

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
# don't want it to go in include/UnitTest++?
# define UTPP_INSTALL_DESTINATION
if (NOT DEFINED UTPP_INSTALL_DESTINATION)
    if(${UTPP_USE_PLUS_SIGN})
    	set (UTPP_INSTALL_DESTINATION "include/UnitTest++")
    else()
    	set (UTPP_INSTALL_DESTINATION "include/UnitTestPP")
    endif()
endif()

install(TARGETS UnitTestPP DESTINATION lib)
install(FILES ${headers_} DESTINATION ${UTPP_INSTALL_DESTINATION})