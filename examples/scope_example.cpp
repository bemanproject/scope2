// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/scope/scope_guard_demo.hpp>

#include <cstdlib>
#include <iostream>
#include <string_view>

namespace scope = beman::scope::demo;

void print_exit_status(std::string_view name, bool exit_status, bool did_throw) {
    std::cout << name << ":\n";
    std::cout << "  Exception thrown: " << (did_throw ? "yes" : "no") << "\n";
    std::cout << "  Exit status:      " << (exit_status ? "handled" : "no handling") << "\n\n";
}

// Randomly throw an exception (50% chance)
void maybe_throw() {
    if (std::rand() >= RAND_MAX / 2) {
        throw std::exception{};
    }
}

void standard_exit() {
    bool exit_status{false};
    bool did_throw{false};

    // Manual handling at "end of scope"
    try {
        maybe_throw();
        exit_status = true;
    } catch (...) {
        did_throw = true;
    }
    print_exit_status("Manual handling", exit_status, did_throw);

    // Using scope_exit: runs on scope exit (success or exception)
    exit_status = did_throw = false;
    try {
        auto guard = scope::scope_exit([&] { exit_status = true; });
        maybe_throw();
    } catch (...) {
        did_throw = true;
    }
    print_exit_status("scope_exit", exit_status, did_throw);

    // Using scope_fail: runs only if an exception occurs
    exit_status = did_throw = false;
    try {
        auto guard = scope::scope_fail([&] { exit_status = true; });
        maybe_throw();
    } catch (...) {
        did_throw = true;
    }
    print_exit_status("scope_fail", exit_status, did_throw);

    // Using scope_success: runs only if no exception occurs
    exit_status = did_throw = false;
    try {
        auto guard = scope::scope_success([&] { exit_status = true; });
        maybe_throw();
    } catch (...) {
        did_throw = true;
    }
    print_exit_status("scope_success", exit_status, did_throw);
}

void releasable_exit() {
    bool exit_status{false};
    bool did_throw{false};

    // Manual handling at "end of scope"
    try {
        maybe_throw();
        exit_status = true;
    } catch (...) {
        did_throw = true;
    }
    print_exit_status("Manual handling", exit_status, did_throw);

    // Using scope_exit: runs on scope exit (success or exception)
    exit_status = did_throw = false;
    try {
        auto guard = scope::scope_exit_releasable([&] { exit_status = true; });
        guard.release();
        maybe_throw();
    } catch (...) {
        did_throw = true;
    }
    print_exit_status("scope_exit", exit_status, did_throw);

    // Using scope_fail: runs only if an exception occurs
    exit_status = did_throw = false;
    try {
        auto guard = scope::scope_fail_releasable([&] { exit_status = true; });
        guard.release();
        maybe_throw();
    } catch (...) {
        did_throw = true;
    }
    print_exit_status("scope_fail", exit_status, did_throw);

    // Using scope_success: runs only if no exception occurs
    exit_status = did_throw = false;
    try {
        auto guard = scope::scope_success_releasable([&] { exit_status = true; });
        guard.release();
        maybe_throw();
    } catch (...) {
        did_throw = true;
    }
    print_exit_status("scope_success", exit_status, did_throw);
}

class CancelableAction {
    bool m_can_invoke = true;

  public:
    void cancel() { m_can_invoke = false; }

    void uncancel() { m_can_invoke = true; }

    bool can_invoke() const { return m_can_invoke; }
};

void cancelable_exit() {

    bool exit_status{false};
    bool did_throw{false};

    try {
        auto guard = scope::scope_guard([&] { exit_status = true; }, CancelableAction{});
        guard.cancel();
        maybe_throw();
    } catch (...) {
        did_throw = true;
    }

    print_exit_status("cancelable", exit_status, did_throw);
}

int main() {
    standard_exit();
    releasable_exit();

    cancelable_exit();
}