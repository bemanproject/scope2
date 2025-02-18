// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/scope/scope.hpp>

// define only in one cpp file
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// if this compiled and no exceptions are thrown, below tests will complete successfully

// This is testing the "language" - the ways in which we want to "start" the scope guard.

TEST_CASE("scope_guard") {
    SECTION("Constructing") {

        auto exit_guard = beman::scope::scope_guard{[] {}, [] { return true; }};
        // vvv doesn't compile (as planned)- NOT releasable
        // exit_guard.release();

        REQUIRE(sizeof(decltype(exit_guard)) == 1);

        // vvv Doesn't compile (as planned), return must be bool
        // auto exit_guard2 = beman::scope::scope_guard{[] {}, [] { return 1; }};

        REQUIRE(true);
    }
}

TEST_CASE("scope_exit") {

    SECTION("Constructing") {
        beman::scope::scope_exit exit_guard1([] {});
        // beman::scope::scope_exit exit_guard2 = [] {};   // can't do: no conversion
        // beman::scope::scope_exit exit_guard3 = {[] {}}; // can't do: explict constructor

        auto exit_guard4 = beman::scope::scope_exit([] {});

        auto exit_guard5 = beman::scope::scope_guard{[] {}, [] { return true; }};

        // exit_guard5.release(); // doesn't compile (as planned)- NOT releasable

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
    SECTION("Using with release") {
        SECTION("Exception thrown") {
            bool is_exception_thrown  = false;
            bool is_exit_func_invoked = false;

            try {
                beman::scope::scope_exit exit_guard([&] { is_exit_func_invoked = true; });

                exit_guard.release();

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
                beman::scope::scope_exit exit_guard([&] { is_exit_func_invoked = true; });

                exit_guard.release();

            } catch (...) {
                is_exception_thrown = true;
            }

            REQUIRE_FALSE(is_exception_thrown);
            REQUIRE(is_exit_func_invoked == false);
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

    SECTION("Using with release") {
        SECTION("Exception thrown") {
            bool is_exception_thrown  = false;
            bool is_exit_func_invoked = false;

            try {
                beman::scope::scope_fail exit_guard([&] { is_exit_func_invoked = true; });

                throw std::exception{};

                exit_guard.release();

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

                exit_guard.release();

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

        auto exit_gaurd5 = beman::scope::scope_guard{[] {}, [] { return true; }};

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

    SECTION("Using with release") {
        SECTION("Exception thrown") {
            bool is_exception_thrown  = false;
            bool is_exit_func_invoked = false;

            try {
                beman::scope::scope_success exit_guard([&] { is_exit_func_invoked = true; });

                exit_guard.release();

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

                exit_guard.release();

            } catch (...) {
                is_exception_thrown = true;
            }

            REQUIRE_FALSE(is_exception_thrown);
            REQUIRE(is_exit_func_invoked == false);
        }
    }
}