# CMakeList.txt : CMake project for CMakeKola, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("CMakeKola")

# Add source to this project's executable.
add_executable (CMakeKola "main.cpp"  "KConsole.h" "KParameters.h" "KWorkRegistry.h" "KHour.h" "KDate.h" "KWorkday.h" "UnitTest.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeKola PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
