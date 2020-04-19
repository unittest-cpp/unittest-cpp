cmake_minimum_required(VERSION 3.0)
project(UnitTest++ VERSION 2.1.0)

option(UTPP_USE_PLUS_SIGN
    "Set this to OFF if you wish to use '-cpp' instead of '++' in lib/include paths"
    ON)
option(UTPP_INCLUDE_TESTS_IN_BUILD
    "Set this to OFF if you do not wish to automatically build or run unit tests as part of the default cmake --build"
    ON)
option(UTPP_AMPLIFY_WARNINGS
    "Set this to OFF if you wish to use CMake default warning levels; should generally only use to work around support issues for your specific compiler"
    ON)

set(LIB_SUFFIX "" CACHE STRING "Identifier to add to end of lib directory name e.g. 64 for lib64")

if (MSVC)
	# CHECK_CXX_COMPILER_FLAG could be used
	# but MSVC version is preferred for feature requirements
	if (MSVC14 OR MSVC12)
		# has the support we need
	else()
		message(STATUS "The MSVC compiler version does not support UnitTest++ C++11 features.")
	endif()
else()
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX14)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
    endif()
endif()

# up warning level for project
if (${UTPP_AMPLIFY_WARNINGS})
    # instead of getting compiler specific, we're going to try making an assumption that an existing /W# means
    # we are dealing with an MSVC or MSVC-like compiler (e.g. Intel on Windows)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
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
add_library(UnitTest++::UnitTest++ ALIAS UnitTest++)


if(${UTPP_USE_PLUS_SIGN})
	set_target_properties(UnitTest++ PROPERTIES OUTPUT_NAME UnitTest++)
endif()


# build the test runner
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp tests/*.h)
source_group( "" FILES ${TEST_SRCS})
add_executable(TestUnitTest++ ${TEST_SRCS})


if(${UTPP_USE_PLUS_SIGN})
	set_target_properties(TestUnitTest++ PROPERTIES OUTPUT_NAME TestUnitTest++)
endif()

target_link_libraries(TestUnitTest++ 
	PUBLIC
		UnitTest++::UnitTest++
	)

# run unit tests as post build step
add_custom_command(TARGET TestUnitTest++
    POST_BUILD COMMAND TestUnitTest++
    COMMENT "Running unit tests")

if(NOT ${UTPP_INCLUDE_TESTS_IN_BUILD})
    set_target_properties(TestUnitTest++ PROPERTIES EXCLUDE_FROM_ALL 1)
endif()

# add install targets
# need a custom install path?
# define CMAKE_INSTALL_PREFIX to change root folder
if(${UTPP_USE_PLUS_SIGN})
	set (UTPP_INSTALL_DESTINATION "include/UnitTest++")
else()
	set (UTPP_INSTALL_DESTINATION "include/UnitTestPP")
endif()

target_include_directories( UnitTest++
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/UnitTest++>
	) 
set_target_properties(UnitTest++ PROPERTIES DEBUG_POSTFIX "-d")
set_target_properties(TestUnitTest++ PROPERTIES DEBUG_POSTFIX "-d")

set(config_install_dir_ lib${LIB_SUFFIX}/cmake/${PROJECT_NAME})
set(targets_export_name_ "${PROJECT_NAME}Targets")
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	cmake/UnitTest++ConfigVersion.cmake
	VERSION ${UnitTest++_VERSION}
	COMPATIBILITY SameMajorVersion
	)

install(TARGETS UnitTest++ EXPORT "${targets_export_name_}" DESTINATION lib${LIB_SUFFIX})
install(FILES ${headers_} DESTINATION ${UTPP_INSTALL_DESTINATION})
install(FILES ${platformHeaders_} DESTINATION ${UTPP_INSTALL_DESTINATION}/${platformDir_})
install(FILES 
	cmake/UnitTest++Config.cmake 
	${CMAKE_CURRENT_BINARY_DIR}/cmake/UnitTest++ConfigVersion.cmake
	DESTINATION "${config_install_dir_}")
install(EXPORT "${targets_export_name_}" NAMESPACE "UnitTest++::" DESTINATION "${config_install_dir_}")

set(prefix      ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX}/bin)
set(libdir      ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
set(includedir  ${CMAKE_INSTALL_PREFIX}/include/UnitTest++)
configure_file("UnitTest++.pc.in" "UnitTest++.pc" @ONLY)
if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    set(pkgconfdir  ${CMAKE_INSTALL_PREFIX}/libdata/pkgconfig)
else()
    set(pkgconfdir  ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/pkgconfig)
endif()
install(FILES       "${CMAKE_CURRENT_BINARY_DIR}/UnitTest++.pc"
        DESTINATION "${pkgconfdir}")
