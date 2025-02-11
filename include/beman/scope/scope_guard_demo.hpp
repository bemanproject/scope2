// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <concepts>
#include <exception> // needed for scope_fail & scope_success

namespace beman::scope::demo {

template <typename T>
struct Wrapper {
    T val;
};

template <std::invocable ExitCallable, typename... Checkers> //
class scope_guard : public Checkers... {
    ExitCallable m_exit_func;

  public:
    scope_guard(ExitCallable exit_func, Checkers&&... checkers)
        : Checkers{std::move(checkers)}..., //
          m_exit_func(std::move(exit_func)) //
    {}

    ~scope_guard() {
        if constexpr (sizeof...(Checkers) > 0) {
            if ((call_check<Checkers>(*this) && ...)) {
                m_exit_func();
            }
        } else {
            std::invoke(m_exit_func);
        }
    }

  private:
    template <typename T>
    bool call_check(const T& obj) const {
        if constexpr (requires(T t) {
                          { t.can_invoke() } -> std::convertible_to<bool>;
                      }) {
            return obj.can_invoke();
        } else if constexpr (requires {
                                 { T::operator()() } -> std::convertible_to<bool>;
                             }) {
            return T::operator()();
        } else if constexpr (requires(T t) {
                                 { t.operator()() } -> std::convertible_to<bool>;
                             }) {
            return obj();
        } else {
            return true; // Default behavior if no check function is available
        }
    }
};

[[nodiscard]] inline auto scope_exit(auto&& exit_func) { //
    return scope_guard{exit_func};
}

[[nodiscard]] inline auto scope_success(auto&& success_func) {
    return scope_guard{
        success_func, //
        [exception_count = std::uncaught_exceptions()] { return exception_count >= std::uncaught_exceptions(); }};
}

[[nodiscard]] inline auto scope_fail(auto&& fail_func) {
    return scope_guard{
        fail_func, //
        [exception_count = std::uncaught_exceptions()] { return exception_count < std::uncaught_exceptions(); }};
}

class ReleasableAction {
    bool m_can_invoke = true;

  public:
    void release() { m_can_invoke = false; }

    void unrelease() { m_can_invoke = true; }

    bool can_invoke() const { return m_can_invoke; }
};

[[nodiscard]] inline auto scope_exit_releasable(auto&& exit_func) { //
    return scope_guard{exit_func, ReleasableAction{}};
}

[[nodiscard]] inline auto scope_success_releasable(auto&& success_func) {
    return scope_guard{
        success_func, //
        [exception_count = std::uncaught_exceptions()] { return exception_count >= std::uncaught_exceptions(); },
        ReleasableAction{}};
}

[[nodiscard]] inline auto scope_fail_releasable(auto&& fail_func) {
    return scope_guard{
        fail_func, //
        [exception_count = std::uncaught_exceptions()] { return exception_count < std::uncaught_exceptions(); },
        ReleasableAction{}};
}

} // namespace beman::scope::demo