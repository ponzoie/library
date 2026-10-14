#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <map>
#include <optional>
#include <type_traits>
#include <utility>

namespace ponzoie {

// A union of integer half-open intervals. Adjacent intervals are coalesced.
template <class T = long long>
class interval_set {
    static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool> && sizeof(T) <= 8,
                  "interval_set requires an integer type of at most 64 bits");

public:
    using value_type = T;
    using length_type = __int128_t;
    using interval_type = std::pair<T, T>;

    bool empty() const { return intervals_.empty(); }
    std::size_t size() const { return intervals_.size(); }
    length_type covered_length() const { return covered_length_; }
    const std::map<T, T>& intervals() const { return intervals_; }

    // Returns the number of newly covered integers.
    length_type insert(T l, T r) {
        assert(l <= r);
        if (l == r) return 0;
        auto it = intervals_.lower_bound(l);
        if (it != intervals_.begin() && std::prev(it)->second >= l) --it;
        T merged_l = l, merged_r = r;
        length_type removed = 0;
        while (it != intervals_.end() && it->first <= merged_r) {
            merged_l = std::min(merged_l, it->first);
            merged_r = std::max(merged_r, it->second);
            removed += length(it->first, it->second);
            it = intervals_.erase(it);
        }
        intervals_.emplace(merged_l, merged_r);
        const length_type added = length(merged_l, merged_r) - removed;
        covered_length_ += added;
        return added;
    }

    // Returns the number of integers that became uncovered.
    length_type erase(T l, T r) {
        assert(l <= r);
        if (l == r) return 0;
        auto it = intervals_.upper_bound(l);
        if (it != intervals_.begin() && std::prev(it)->second > l) --it;
        length_type removed = 0;
        while (it != intervals_.end() && it->first < r) {
            const auto [a, b] = *it;
            removed += length(std::max(a, l), std::min(b, r));
            it = intervals_.erase(it);
            if (a < l) intervals_.emplace(a, l);
            if (r < b) {
                intervals_.emplace(r, b);
                break;
            }
        }
        covered_length_ -= removed;
        return removed;
    }

    bool contains(T x) const { return covered_by(x).has_value(); }

    bool contains(T l, T r) const {
        assert(l <= r);
        if (l == r) return true;
        const auto interval = covered_by(l);
        return interval && r <= interval->second;
    }

    // Returns the maximal stored interval containing x, if there is one.
    std::optional<interval_type> covered_by(T x) const {
        auto it = intervals_.upper_bound(x);
        if (it == intervals_.begin()) return std::nullopt;
        --it;
        if (x < it->second) return interval_type{it->first, it->second};
        return std::nullopt;
    }

    // Smallest uncovered integer greater than or equal to x.
    T mex(T x = T{}) const {
        const auto interval = covered_by(x);
        return interval ? interval->second : x;
    }

    void clear() {
        intervals_.clear();
        covered_length_ = 0;
    }

private:
    std::map<T, T> intervals_;
    length_type covered_length_ = 0;

    static length_type length(T l, T r) {
        return static_cast<length_type>(r) - static_cast<length_type>(l);
    }
};

}  // namespace ponzoie
