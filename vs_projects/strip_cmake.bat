@echo off
setlocal
set slnDir_=%1

rem TODO: Use VS automation to remove CMake projects

rem This batch file tries to delete things from the sub-folders
rem that may or may not be there based on the Visual Studio
rem version in play. I chose to make it quick and dumb so that
rem it can be called on each project folder with no special
rem logic.

pushd %slnDir_%
del /Q ALL_BUILD.* check.* cmake_install.cmake CMakeCache.txt CTestTestfile.cmake
del /Q INSTALL.* RUN_TESTS.*
rmdir /s /q CMakeFiles
popd