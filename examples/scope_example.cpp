// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/scope/scope.hpp>

#include <cstdlib>
#include <format>
#include <iostream>
#include <string_view>

namespace scope = beman::scope;

void print_exit_status(std::string_view name, bool exit_status, bool did_throw) {
    std::cout << std::format("{}:\n"
                             "  Exception thrown: {}\n"
                             "  Exit function:    {}\n"
                             "\n",
                             name,
                             (did_throw ? "yes" : "no"),
                             (exit_status ? "invoked" : "not invoked"));
}

// Randomly throw an exception (50% chance)
void maybe_throw() {
    if (std::rand() >= RAND_MAX / 2) {
        throw std::exception{};
    }
}

int main() {
    bool is_exit_func_invoked {false};
    bool is_exception_thrown {false};

    // Manual handling at "end of scope"
    try {
        maybe_throw();
        is_exit_func_invoked = true;
    } catch (...) {
        is_exception_thrown = true;
    }
    print_exit_status("Manual handling", is_exit_func_invoked, is_exception_thrown);

    // Using scope_exit: runs on scope exit (success or exception)
    is_exit_func_invoked = is_exception_thrown = false;
    try {
        auto guard = scope::scope_exit{[&] { is_exit_func_invoked = true; }};
        maybe_throw();
    } catch (...) {
        is_exception_thrown = true;
    }
    print_exit_status("scope_exit", is_exit_func_invoked, is_exception_thrown);

    // Using scope_fail: runs only if an exception occurs
    is_exit_func_invoked = is_exception_thrown = false;
    try {
        auto guard = scope::scope_fail{[&] { is_exit_func_invoked = true; }};
        maybe_throw();
    } catch (...) {
        is_exception_thrown = true;
    }
    print_exit_status("scope_fail", is_exit_func_invoked, is_exception_thrown);

    // Using scope_success: runs only if no exception occurs
    is_exit_func_invoked = is_exception_thrown = false;
    try {
        auto guard = scope::scope_success{[&] { is_exit_func_invoked = true; }};
        maybe_throw();
    } catch (...) {
        is_exception_thrown = true;
    }
    print_exit_status("scope_success", is_exit_func_invoked, is_exception_thrown);
}
