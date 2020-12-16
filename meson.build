#
# @project author: Noel Llopis and Charles Nicholson
# @script author :  Michael Brockus
# @gmail:   michaelbrockus@gmail.com
# @license: Apache 2.0 
#         : http://www.apache.org/licenses/LICENSE-2.0
#------------------------------------------------------------------
project('UnitTest++', 'cpp', meson_version: '>=0.53.0')

subdir('UnitTest++')

unittest_cpp_dep = declare_dependency(link_with: unittest_cpp_lib, include_directories: unittest_cpp_dir)
