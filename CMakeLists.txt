cmake_minimum_required(VERSION 2.8.1)
project(UnitTest++)

option(UTPP_USE_PLUS_SIGN
    "Set this to OFF if you wish to use '-cpp' instead of '++' in lib/include paths"
    ON)
option(UTPP_INCLUDE_TESTS_IN_BUILD
    "Set this to OFF if you do not wish to automatically build or run unit tests as part of the default cmake --build"
    ON)
option(UTPP_AMPLIFY_WARNINGS
    "Set this to OFF if you wish to use CMake default warning levels; should generally only use to work around support issues for your specific compiler"
    ON)
    
if(BORLAND)
    set(UTPP_USE_PLUS_SIGN OFF)
    string(APPEND CMAKE_CXX_STANDARD_LIBRARIES " cp32mti.lib Vcl50.bpi Vclx50.bpi bcbsmp50.bpi Qrpt50.bpi Vcldb50.bpi Vclbde50.bpi ibsmp50.bpi vcldbx50.bpi TeeUI50.bpi TeeDB50.bpi Tee50.bpi TeeQR50.bpi VCLIB50.bpi bcbie50.bpi vclie50.bpi Inetdb50.bpi Inet50.bpi NMFast50.bpi dclocx50.bpi bcb2kaxserver50.bpi Memmgr.Lib ")
endif()
    
if(MSVC14 OR MSVC12)
    # has the support we need
else()
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX14)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
            message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

# up warning level for project
if (${UTPP_AMPLIFY_WARNINGS})
    # instead of getting compiler specific, we're going to try making an assumption that an existing /W# means
    # we are dealing with an MSVC or MSVC-like compiler (e.g. Intel on Windows)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
	elseif(BORLAND)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w-par")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
    endif()
endif()

# get the main sources
file(GLOB headers_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} UnitTest++/*.h)
file(GLOB sources_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} UnitTest++/*.cpp)
source_group("" FILES ${headers_} ${sources_})

# get platform specific sources
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    set(platformDir_ Win32)
else()
    set(platformDir_ Posix)
endif(WIN32)

file(GLOB platformHeaders_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} UnitTest++/${platformDir_}/*.h)
file(GLOB platformSources_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} UnitTest++/${platformDir_}/*.cpp)
source_group(${platformDir_} FILES ${platformHeaders_} ${platformSources_})

# create the lib
add_library(UnitTest++ STATIC ${headers_} ${sources_} ${platformHeaders_} ${platformSources_})

if(${UTPP_USE_PLUS_SIGN})
	set_target_properties(UnitTest++ PROPERTIES OUTPUT_NAME UnitTest++)
else()
	set_target_properties(UnitTest++ PROPERTIES OUTPUT_NAME UnitTestpp)
endif()


if(${UTPP_USE_PLUS_SIGN})
    set(UTPP_TEST_NAME TestUnitTest++)
else()
    set(UTPP_TEST_NAME TestUnitTestPP)
endif()

# build the test runner
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp tests/*.h)
source_group( "" FILES ${TEST_SRCS})
add_executable(${UTPP_TEST_NAME} ${TEST_SRCS})
include_directories(.)

set_target_properties(${UTPP_TEST_NAME} PROPERTIES OUTPUT_NAME ${UTPP_TEST_NAME})

target_link_libraries(${UTPP_TEST_NAME} UnitTest++)

# run unit tests as post build step
add_custom_command(TARGET ${UTPP_TEST_NAME}
    POST_BUILD COMMAND ${UTPP_TEST_NAME}
    COMMENT "Running unit tests")

if(NOT ${UTPP_INCLUDE_TESTS_IN_BUILD})
    set_target_properties(${UTPP_TEST_NAME} PROPERTIES EXCLUDE_FROM_ALL 1)
endif()

# add install targets
# need a custom install path?
# define CMAKE_INSTALL_PREFIX to change root folder
if(${UTPP_USE_PLUS_SIGN})
	set (UTPP_INSTALL_DESTINATION "include/UnitTest++")
else()
	set (UTPP_INSTALL_DESTINATION "include/UnitTestPP")
endif()

set(config_install_dir_ lib/cmake/${PROJECT_NAME})
set(targets_export_name_ "${PROJECT_NAME}Targets")

install(TARGETS UnitTest++ EXPORT "${targets_export_name_}" DESTINATION lib)
install(FILES ${headers_} DESTINATION ${UTPP_INSTALL_DESTINATION})
install(FILES ${platformHeaders_} DESTINATION ${UTPP_INSTALL_DESTINATION}/${platformDir_})
install(FILES cmake/UnitTest++Config.cmake DESTINATION "${config_install_dir_}")
install(EXPORT "${targets_export_name_}" DESTINATION "${config_install_dir_}")
