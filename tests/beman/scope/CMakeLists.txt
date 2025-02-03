# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(GoogleTest)

add_executable(beman.scope.tests.identity)
target_sources(beman.scope.tests.identity PRIVATE identity.test.cpp)
target_link_libraries(
    beman.scope.tests.identity
    PRIVATE beman::scope GTest::gtest GTest::gtest_main
)

gtest_add_tests(beman.scope.tests.identity "" AUTO)
