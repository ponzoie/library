#pragma once

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>
#include <atcoder/fenwicktree>

namespace ponzoie {

// 最長増加部分列の添字。strict=false なら広義単調増加。
template <class T>
std::vector<int> lis_indices(const std::vector<T>& a, bool strict = true) {
    std::vector<T> tails;
    std::vector<int> ends, prev(a.size(), -1);
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        auto it = strict ? std::lower_bound(tails.begin(), tails.end(), a[i])
                         : std::upper_bound(tails.begin(), tails.end(), a[i]);
        int k = static_cast<int>(it - tails.begin());
        if (k > 0) prev[i] = ends[k - 1];
        if (it == tails.end()) { tails.push_back(a[i]); ends.push_back(i); }
        else { *it = a[i]; ends[k] = i; }
    }
    std::vector<int> result;
    for (int i = ends.empty() ? -1 : ends.back(); i != -1; i = prev[i])
        result.push_back(i);
    std::reverse(result.begin(), result.end());
    return result;
}

template <class T> int lis_length(const std::vector<T>& a, bool strict = true) {
    return static_cast<int>(lis_indices(a, strict).size());
}

template <class T> long long inversion_number(const std::vector<T>& a) {
    std::vector<T> values = a;
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
    atcoder::fenwick_tree<long long> bit(static_cast<int>(values.size()));
    long long result = 0;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        int p = static_cast<int>(std::lower_bound(values.begin(), values.end(), a[i]) - values.begin());
        result += i - bit.sum(0, p + 1);
        bit.add(p, 1);
    }
    return result;
}

template <class Sequence>
auto run_length_encode(const Sequence& a) {
    using T = typename Sequence::value_type;
    std::vector<std::pair<T, int>> result;
    for (const auto& x : a) {
        if (result.empty() || result.back().first != x) result.emplace_back(x, 1);
        else ++result.back().second;
    }
    return result;
}

} // namespace ponzoie
