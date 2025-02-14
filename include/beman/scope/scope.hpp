// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_SCOPE_HPP
#define BEMAN_SCOPE_HPP

#include <concepts>
#include <exception>

namespace beman::scope {

// -- 7.6.7 Feature test macro --
//
//        __cpp_lib_scope
//

//=========================================================

// -- 7.5.2 Scope guard class templates [scope.scope_guard] --
//
//        template <class EF>
//        class scope_guard {
//        public:
//            template <class EFP>
//            explicit scope_guard(EFP&& f) noexcept(see below);
//
//            scope_guard(scope_guard&& rhs) noexcept(see below);
//
//            scope_guard(const scope_guard&)            = delete;
//            scope_guard& operator=(const scope_guard&) = delete;
//            scope_guard& operator=(scope_guard&&)      = delete;
//
//            ~scope_guard() noexcept(see below);
//
//            void release() noexcept;
//
//        private:
//            EF   exit_function;                               // exposition only
//            bool execute_on_destruction{true};                // exposition only
//            int  uncaught_on_creation{uncaught_exceptions()}; // exposition only
//        };
//
//        template <class EF>
//        scope_guard(EF) -> scope_guard<EF>;
//

//=========================================================

template <typename F, typename R, typename... Args>
concept invocable_return = std::invocable<F, Args...> && std::same_as<std::invoke_result_t<F, Args...>, R>;

//=========================================================

struct ExecuteAlways;

//=========================================================

template <invocable_return<void> ExitFunc, invocable_return<bool> ExecuteCondition = ExecuteAlways>
class scope_guard {
  public:
    explicit scope_guard(ExitFunc&& func) /*noexcept(see below)*/
        : m_exit_func(std::move(func))    //
    {}

    scope_guard(scope_guard&& rhs) noexcept
        : m_exit_func{std::move(rhs)}, m_invoke_condition_func{std::move(rhs.m_invoke_condition_func)} {}

    scope_guard(const scope_guard&)            = delete;
    scope_guard& operator=(const scope_guard&) = delete;
    scope_guard& operator=(scope_guard&&)      = delete;

    ~scope_guard() /*noexcept(see below)*/ {
        if (can_invoke_check(m_invoke_condition_func)) {
            m_exit_func();
        }
    }

    void release() noexcept {
        // Needs implementation
    }

  private:
    ExitFunc                               m_exit_func;
    [[no_unique_address]] ExecuteCondition m_invoke_condition_func;

    template <typename T>
    bool can_invoke_check(const T& obj) const {
        if constexpr (requires(T /*t*/) {
                          { T::can_invoke() } -> std::convertible_to<bool>;
                      }) {
            return T::can_invoke();
        } else if constexpr (requires(T t) {
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

//=========================================================

template <std::invocable ExitFunc>
scope_guard(ExitFunc) -> scope_guard<ExitFunc>;

//=========================================================

// -- 7.5.1 Header <scope> synopsis [scope.syn] --
//
//        namespace std {
//        template <class EF>
//        class scope_exit;
//
//        template <class EF>
//        class scope_fail;
//
//        template <class EF>
//        class scope_success;

//=========================================================

struct ExecuteAlways {
    [[nodiscard]] constexpr bool operator()() const { return true; }
};

struct ExecuteWhenNoException {

    [[nodiscard]] bool operator()() const { return uncaught_on_creation >= std::uncaught_exceptions(); }

  private:
    int uncaught_on_creation = std::uncaught_exceptions();
};

struct ExecuteOnlyWhenException {

    [[nodiscard]] bool operator()() const { return uncaught_on_creation < std::uncaught_exceptions(); }

  private:
    int uncaught_on_creation = std::uncaught_exceptions();
};

//=========================================================

template <class ExitFunc>
using scope_exit = scope_guard<ExitFunc>;

template <class ExitFunc>
using scope_success = scope_guard<ExitFunc, ExecuteWhenNoException>;

template <class ExitFunc>
using scope_fail = scope_guard<ExitFunc, ExecuteOnlyWhenException>;

//=========================================================

//
//        template <class R, class D>
//        class unique_resource;
//
//        // factory function
//        template <class R, class D, class S = decay_t<R>>
//        unique_resource<decay_t<R>, decay_t<D>>
//        make_unique_resource_checked(R&& r, const S& invalid, D&& d) noexcept(see below);
//        } // namespace std
//

//=========================================================

// -- 7.6.1 Class template unique_resource [scope.unique_resource.class] --
//
//        template <class R, class D>
//        class unique_resource {
//        public:
//            unique_resource();
//            template <class RR, class DD>
//            unique_resource(RR&& r, DD&& d) noexcept(see below);
//            unique_resource(unique_resource&& rhs) noexcept(see below);
//            ~unique_resource();
//            unique_resource& operator=(unique_resource&& rhs) noexcept(see below);
//            void             reset() noexcept;
//            template <class RR>
//            void      reset(RR&& r);
//            void      release() noexcept;
//            const R&  get() const noexcept;
//            see below operator*() const noexcept;
//            R         operator->() const noexcept;
//            const D&  get_deleter() const noexcept;
//
//        private:
//            using R1 = conditional_t<is_reference_v<R>,                        //
//                                    reference_wrapper<remove_reference_t<R>>, //
//                                    R>;                                       // exposition only
//
//            R1   resource;               // exposition only
//            D    deleter;                // exposition only
//            bool execute_on_reset{true}; // exposition only
//        };
//
//        template <typename R, typename D>
//        unique_resource(R, D) -> unique_resource<R, D>;

} // namespace beman::scope

#endif // BEMAN_SCOPE_HPP
