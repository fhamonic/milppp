#ifndef MIPPP_LINEAR_EXPRESSION_OPERATIONS_HPP
#define MIPPP_LINEAR_EXPRESSION_OPERATIONS_HPP

#include <functional>

#include <iostream>

#include <range/v3/view/concat.hpp>
#include <range/v3/view/transform.hpp>

#include "mippp/concepts/linear_expression.hpp"

namespace fhamonic {
namespace mippp {

template <typename T>
void print_type(void) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template <linear_expression_c E1, linear_expression_c E2>
requires linear_expression_c<typename std::remove_reference<E1>::type> &&
    linear_expression_c<typename std::remove_reference<E2>::type> &&
    std::same_as<typename E1::var_id_t, typename E2::var_id_t> &&
    std::same_as<typename E1::scalar_t, typename E2::scalar_t>
class linear_expression_add {
public:
    using var_id_t = typename E1::var_id_t;
    using scalar_t = typename E1::scalar_t;

private:
    const E1 && _lhs;
    const E2 && _rhs;

public:
    constexpr linear_expression_add(const E1 && e1, const E2 && e2)
        : _lhs(std::forward<const E1>(e1)), _rhs(std::forward<const E2>(e2)) {
        print_type<const E1 &&>();
        print_type<const E2 &&>();
    };

    constexpr auto variables() const noexcept {
        return ranges::views::concat(_lhs.variables(), _rhs.variables());
    }
    constexpr auto coefficients() const noexcept {
        return ranges::views::concat(_lhs.coefficients(), _rhs.coefficients());
    }
    constexpr scalar_t constant() const noexcept {
        return _lhs.constant() + _rhs.constant();
    }
};

template <linear_expression_c E1, linear_expression_c E2>
requires linear_expression_c<typename std::remove_reference<E1>::type> &&
    linear_expression_c<typename std::remove_reference<E2>::type> &&
    std::same_as<typename E1::var_id_t, typename E2::var_id_t> &&
    std::same_as<typename E1::scalar_t, typename E2::scalar_t>
class linear_expression_sub {
public:
    using var_id_t = typename E1::var_id_t;
    using scalar_t = typename E1::scalar_t;

private:
    const E1 && _lhs;
    const E2 && _rhs;

public:
    constexpr linear_expression_sub(const E1 && e1, const E2 && e2)
        : _lhs(std::forward<const E1>(e1)), _rhs(std::forward<const E2>(e2)) {
        print_type<const E1 &&>();
        print_type<const E2 &&>();
    };

    constexpr auto variables() const noexcept {
        return ranges::views::concat(_lhs.variables(), _rhs.variables());
    }
    constexpr auto coefficients() const noexcept {
        return ranges::views::concat(
            _lhs.coefficients(),
            ranges::views::transform(_rhs.coefficients(),
                                     std::negate<scalar_t>()));
    }
    constexpr scalar_t constant() const noexcept {
        return _lhs.constant() + _rhs.constant();
    }
};

template <typename E>
requires linear_expression_c<typename std::remove_reference<E>::type>
class linear_expression_negate {
public:
    using var_id_t = typename E::var_id_t;
    using scalar_t = typename E::scalar_t;

private:
    const E && _expr;

public:
    explicit constexpr linear_expression_negate(const E && e)
        : _expr(std::forward<const E>(e)){};

    constexpr auto variables() const noexcept { return _expr.variables(); }
    constexpr auto coefficients() const noexcept {
        return ranges::views::transform(_expr.coefficients(),
                                        std::negate<scalar_t>());
    }
    constexpr scalar_t constant() const noexcept { return -_expr.constant(); }
};

template <typename E>
requires linear_expression_c<typename std::remove_reference<E>::type>
class linear_expression_scalar_add {
public:
    using var_id_t = typename E::var_id_t;
    using scalar_t = typename E::scalar_t;

private:
    const E && _expr;
    const scalar_t _scalar;

public:
    constexpr linear_expression_scalar_add(const E && e, const scalar_t c)
        : _expr(std::forward<const E>(e)), _scalar(c){};

    constexpr auto variables() const noexcept { return _expr.variables(); }
    constexpr auto coefficients() const noexcept {
        return _expr.coefficients();
    }
    constexpr scalar_t constant() const noexcept {
        return _expr.constant() + _scalar;
    }
};

template <typename E>
requires linear_expression_c<typename std::remove_reference<E>::type>
class linear_expression_scalar_sub_other_way {
public:
    using var_id_t = typename E::var_id_t;
    using scalar_t = typename E::scalar_t;

private:
    const E && _expr;
    const scalar_t _scalar;

public:
    constexpr linear_expression_scalar_sub_other_way(const E && e, const scalar_t c)
        : _expr(std::forward<const E>(e)), _scalar(c){};

    constexpr auto variables() const noexcept { return _expr.variables(); }
    constexpr auto coefficients() const noexcept {
        return ranges::views::transform(_expr.coefficients(),
                                        std::negate<scalar_t>());
    }
    constexpr scalar_t constant() const noexcept {
        return _expr.constant() + _scalar;
    }
};

template <typename E>
requires linear_expression_c<typename std::remove_reference<E>::type>
class linear_expression_scalar_mul {
public:
    using var_id_t = typename E::var_id_t;
    using scalar_t = typename E::scalar_t;

private:
    const E && _expr;
    const scalar_t _scalar;

public:
    constexpr linear_expression_scalar_mul(const E && e, const scalar_t c)
        : _expr(std::forward<const E>(e)), _scalar(c){};

    constexpr auto variables() const noexcept { return _expr.variables(); }
    constexpr auto coefficients() const noexcept {
        return ranges::views::transform(
            _expr.coefficients(), [scalar = _scalar](auto && coef) -> scalar_t {
                return scalar * coef;
            });
    }
    constexpr scalar_t constant() const noexcept {
        return _scalar * _expr.constant();
    }
};

}  // namespace mippp
}  // namespace fhamonic

#endif  // MIPPP_LINEAR_EXPRESSION_OPERATIONS_HPP