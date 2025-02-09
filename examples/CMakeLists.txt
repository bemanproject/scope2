# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(CMAKE_CXX_STANDARD 20)            # Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # Ensure the standard is required
set(CMAKE_CXX_EXTENSIONS OFF)         # Disable compiler-specific extensions

set(ALL_EXAMPLES scope_example)

message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(beman.scope.examples.${example})
    target_sources(beman.scope.examples.${example} PRIVATE ${example}.cpp)
    target_link_libraries(beman.scope.examples.${example} beman::scope)
endforeach()
