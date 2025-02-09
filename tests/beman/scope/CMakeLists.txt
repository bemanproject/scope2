# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(CMAKE_CXX_STANDARD 20)            # Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # Ensure the standard is required
set(CMAKE_CXX_EXTENSIONS OFF)         # Disable compiler-specific extensions

add_executable(beman.scope.tests.scope)
target_sources(beman.scope.tests.scope PRIVATE scope.test.cpp)

target_link_libraries(beman.scope.tests.scope PRIVATE beman::scope)

add_test(NAME beman.scope.tests.scope COMMAND beman.scope.tests.scope)
