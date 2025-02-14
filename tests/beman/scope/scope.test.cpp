// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/scope/scope.hpp>

// define only in one cpp file
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// if this compiled and no exceptions are thrown, below tests will complete successfully

// This is testing the "language" - the ways in which we want to "start" the scope guard.

TEST_CASE("scope_exit") {

    SECTION("Constructing") {
        beman::scope::scope_exit exit_guard1([] {});
        // beman::scope::scope_exit exit_guard2 = [] {};   // can't do: no conversion
        // beman::scope::scope_exit exit_guard3 = {[] {}}; // can't do: explict constructor

        auto exit_guard4 = beman::scope::scope_exit([] {});

        REQUIRE(true);
    }

    SECTION("Using") {
        SECTION("Exception thrown") {
            bool is_exception_thrown  = false;
            bool is_exit_func_invoked = false;

            try {
                beman::scope::scope_exit exit_guard([&] { is_exit_func_invoked = true; });

                throw std::exception{};
            } catch (...) {
                is_exception_thrown = true;
            }

            REQUIRE(is_exception_thrown);
            REQUIRE(is_exit_func_invoked == true);
        }

        SECTION("NO Exception thrown") {
            bool is_exception_thrown  = false;
            bool is_exit_func_invoked = false;

            try {
                beman::scope::scope_exit exit_guard([&] { is_exit_func_invoked = true; });
            } catch (...) {
                is_exception_thrown = true;
            }

            REQUIRE_FALSE(is_exception_thrown);
            REQUIRE(is_exit_func_invoked == true);
        }
    }
}

TEST_CASE("scope_fail") {

    SECTION("Constructing") {
        beman::scope::scope_fail exit_guard1([] {});
        // beman::scope::scope_fail exit_guard2 = [] {};   // can't do: no conversion
        // beman::scope::scope_fail exit_guard3 = {[] {}}; // can't do: explict constructor

        auto exit_guard4 = beman::scope::scope_fail([] {});

        REQUIRE(true);
    }

    SECTION("Using") {
        SECTION("Exception thrown") {
            bool is_exception_thrown  = false;
            bool is_exit_func_invoked = false;

            try {
                beman::scope::scope_fail exit_guard([&] { is_exit_func_invoked = true; });

                throw std::exception{};
            } catch (...) {
                is_exception_thrown = true;
            }

            REQUIRE(is_exception_thrown);
            REQUIRE(is_exit_func_invoked == true);
        }

        SECTION("NO Exception thrown") {
            bool is_exception_thrown  = false;
            bool is_exit_func_invoked = false;

            try {
                beman::scope::scope_fail exit_guard([&] { is_exit_func_invoked = true; });
            } catch (...) {
                is_exception_thrown = true;
            }

            REQUIRE_FALSE(is_exception_thrown);
            REQUIRE(is_exit_func_invoked == false);
        }
    }
}

TEST_CASE("scope_success") {

    SECTION("Constructing") {
        beman::scope::scope_success exit_guard1([] {});
        // beman::scope::scope_success exit_guard2 = [] {};   // can't do: no conversion
        // beman::scope::scope_success exit_guard3 = {[] {}}; // can't do: explict constructor

        auto exit_guard4 = beman::scope::scope_success([] {});

        REQUIRE(true);
    }

    SECTION("Using") {
        SECTION("Exception thrown") {
            bool is_exception_thrown  = false;
            bool is_exit_func_invoked = false;

            try {
                beman::scope::scope_success exit_guard([&] { is_exit_func_invoked = true; });

                throw std::exception{};
            } catch (...) {
                is_exception_thrown = true;
            }

            REQUIRE(is_exception_thrown);
            REQUIRE(is_exit_func_invoked == false);
        }

        SECTION("NO Exception thrown") {
            bool is_exception_thrown  = false;
            bool is_exit_func_invoked = false;

            try {
                beman::scope::scope_success exit_guard([&] { is_exit_func_invoked = true; });
            } catch (...) {
                is_exception_thrown = true;
            }

            REQUIRE_FALSE(is_exception_thrown);
            REQUIRE(is_exit_func_invoked == true);
        }
    }
}