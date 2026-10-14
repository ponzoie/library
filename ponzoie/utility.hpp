#pragma once

#include <cassert>
#include <numeric>
#include <type_traits>

namespace ponzoie {

template <class T, class U> bool chmin(T& a, const U& b) {
    if (b < a) { a = b; return true; }
    return false;
}
template <class T, class U> bool chmax(T& a, const U& b) {
    if (a < b) { a = b; return true; }
    return false;
}

// [l, r) で false -> true になる最初の位置。存在しなければ r。
template <class Int, class Predicate>
Int first_true(Int l, Int r, Predicate pred) {
    static_assert(std::is_integral_v<Int>);
    assert(l <= r);
    while (l < r) {
        Int mid = std::midpoint(l, r);
        if (pred(mid)) r = mid;
        else l = mid + 1;
    }
    return l;
}

} // namespace ponzoie
