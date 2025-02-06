# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_executable(beman.scope.tests.scope)
target_sources(beman.scope.tests.scope PRIVATE scope.test.cpp)

target_link_libraries(beman.scope.tests.scope PRIVATE beman::scope)

add_test(
    NAME beman.scope.tests.scope
    COMMAND beman.scope.tests.scope
)
