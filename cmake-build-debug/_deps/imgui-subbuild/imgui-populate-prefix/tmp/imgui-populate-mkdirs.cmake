# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "F:/graphicsengine/cmake-build-debug/_deps/imgui-src"
  "F:/graphicsengine/cmake-build-debug/_deps/imgui-build"
  "F:/graphicsengine/cmake-build-debug/_deps/imgui-subbuild/imgui-populate-prefix"
  "F:/graphicsengine/cmake-build-debug/_deps/imgui-subbuild/imgui-populate-prefix/tmp"
  "F:/graphicsengine/cmake-build-debug/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp"
  "F:/graphicsengine/cmake-build-debug/_deps/imgui-subbuild/imgui-populate-prefix/src"
  "F:/graphicsengine/cmake-build-debug/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/graphicsengine/cmake-build-debug/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/graphicsengine/cmake-build-debug/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
