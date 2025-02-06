// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_SCOPE_HPP
#define BEMAN_SCOPE_HPP

namespace beman::scope {

// TODO: Implement
struct scope_exit {
    template <typename F>
    scope_exit(F) {}
    ~scope_exit() {
        // TODO: Cleanup
    }
};

// TODO: Implement
struct scope_fail {
    template <typename F>
    scope_fail(F) {}
    ~scope_fail() {
        // TODO: Cleanup
    }
};

// TODO: Implement
struct scope_success {
    template <typename F>
    scope_success(F) {}
    ~scope_success() {
        // TODO: Cleanup
    }
};

} // namespace beman::scope

#endif // BEMAN_SCOPE_HPP
