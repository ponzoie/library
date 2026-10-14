#ifndef PONZOIE_INTEGER_HPP
#define PONZOIE_INTEGER_HPP

#include <algorithm>
#include <cassert>
#include <concepts>
#include <limits>

namespace ponzoie {

template <std::signed_integral T> constexpr T floor_div(T a, T b) {
    assert(b != 0);
    assert(!(a == std::numeric_limits<T>::min() && b == T(-1)));
    const T q = a / b, r = a % b;
    return q - (r != 0 && ((r < 0) != (b < 0)));
}

template <std::signed_integral T> constexpr T ceil_div(T a, T b) {
    assert(b != 0);
    assert(!(a == std::numeric_limits<T>::min() && b == T(-1)));
    const T q = a / b, r = a % b;
    return q + (r != 0 && ((r < 0) == (b < 0)));
}

// floor(sqrt(n))。浮動小数点も mid*mid も使わない。
inline long long isqrt(long long n) {
    assert(n >= 0);
    long long lo = 0, hi = std::min(n, 3037000500LL) + 1;
    while (hi - lo > 1) {
        const long long mid = lo + (hi - lo) / 2;
        if (mid <= n / mid) lo = mid;
        else hi = mid;
    }
    return lo;
}

} // namespace ponzoie

#endif
