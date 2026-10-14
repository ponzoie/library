#pragma once

#include <cassert>
#include <iterator>
#include <map>
#include <tuple>
#include <vector>

namespace ponzoie {

// 有限区間 [left, right) の区間代入。同じ値の隣接区間を併合する。
template <class Coordinate, class Value> class interval_map {
    Coordinate left_, right_;
    std::map<Coordinate, Value> runs_;
    auto split(Coordinate x) {
        if (x == right_) return runs_.end();
        auto it = std::prev(runs_.upper_bound(x));
        if (it->first == x) return it;
        return runs_.emplace(x, it->second).first;
    }
public:
    interval_map(Coordinate left, Coordinate right, Value value)
        : left_(left), right_(right) {
        assert(left < right);
        runs_.emplace(left, value);
    }
    int interval_count() const { return static_cast<int>(runs_.size()); }
    const Value& get(Coordinate x) const {
        assert(left_ <= x && x < right_);
        return std::prev(runs_.upper_bound(x))->second;
    }
    void assign(Coordinate l, Coordinate r, const Value& value) {
        assert(left_ <= l && l <= r && r <= right_);
        if (l == r) return;
        // value が get() の参照でも、erase によって無効にならないようコピー。
        Value copied = value;
        auto last = split(r);
        auto first = split(l);
        runs_.erase(first, last);
        auto it = runs_.emplace(l, copied).first;
        if (it != runs_.begin() && std::prev(it)->second == it->second) {
            auto prev = std::prev(it);
            runs_.erase(it);
            it = prev;
        }
        auto next = std::next(it);
        if (next != runs_.end() && next->second == it->second) runs_.erase(next);
    }
    template <class Callback>
    void for_each(Coordinate l, Coordinate r, Callback callback) const {
        assert(left_ <= l && l <= r && r <= right_);
        if (l == r) return;
        auto it = std::prev(runs_.upper_bound(l));
        while (l < r) {
            auto next = std::next(it);
            Coordinate end = next == runs_.end() ? right_ : next->first;
            if (r < end) end = r;
            callback(l, end, it->second);
            l = end;
            it = next;
        }
    }
    std::vector<std::tuple<Coordinate, Coordinate, Value>> intervals() const {
        std::vector<std::tuple<Coordinate, Coordinate, Value>> result;
        for_each(left_, right_, [&](Coordinate l, Coordinate r, const Value& v) {
            result.emplace_back(l, r, v);
        });
        return result;
    }
};

} // namespace ponzoie
