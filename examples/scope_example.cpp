// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <cstdlib>
#include <experimental/scope>
#include <iostream>
#include <string_view>

void print_exit_status(std::string_view name, bool exit_status, bool did_throw)
{
    std::cout << name << ":\n";
    std::cout << "  Throwed exception  " << (did_throw ? "yes" : "no") << "\n";
    std::cout << "  Exit status        " << (exit_status ? "finished" : "pending") << "\n\n";
}

// Randomly throw an exception (50% chance)
void maybe_throw()
{
    if (std::rand() >= RAND_MAX / 2)
        throw std::exception{};
}

int main()
{
    bool exit_status{false}, did_throw{false};

    // Manual handling at "end of scope"
    try
    {
        maybe_throw();
        exit_status = true;
    }
    catch (...)
    {
        did_throw = true;
    }
    print_exit_status("Manual handling", exit_status, did_throw);

    // Using scope_exit: runs on scope exit (success or exception)
    exit_status = did_throw = false;
    try
    {
        auto guard = std::experimental::scope_exit{[&] { exit_status = true; }};
        maybe_throw();
    }
    catch (...)
    {
        did_throw = true;
    }
    print_exit_status("scope_exit", exit_status, did_throw);

    // Using scope_fail: runs only if an exception occurs
    exit_status = did_throw = false;
    try
    {
        auto guard = std::experimental::scope_fail{[&] { exit_status = true; }};
        maybe_throw();
    }
    catch (...)
    {
        did_throw = true;
    }
    print_exit_status("scope_fail", exit_status, did_throw);

    // Using scope_success: runs only if no exception occurs
    exit_status = did_throw = false;
    try
    {
        auto guard = std::experimental::scope_success{[&] { exit_status = true; }};
        maybe_throw();
    }
    catch (...)
    {
        did_throw = true;
    }
    print_exit_status("scope_success", exit_status, did_throw);
}
