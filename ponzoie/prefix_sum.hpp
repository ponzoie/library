#pragma once

#include <cassert>
#include <vector>

namespace ponzoie {

template <class T> class prefix_sum {
    std::vector<T> prefix_;
public:
    explicit prefix_sum(const std::vector<T>& a) : prefix_(a.size() + 1, T{}) {
        for (int i = 0; i < static_cast<int>(a.size()); ++i)
            prefix_[i + 1] = prefix_[i] + a[i];
    }
    int size() const { return static_cast<int>(prefix_.size()) - 1; }
    T sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= size());
        return prefix_[r] - prefix_[l];
    }
};

template <class T> class prefix_sum_2d {
    int h_, w_;
    std::vector<std::vector<T>> prefix_;
public:
    explicit prefix_sum_2d(const std::vector<std::vector<T>>& a)
        : h_(static_cast<int>(a.size())),
          w_(a.empty() ? 0 : static_cast<int>(a[0].size())),
          prefix_(h_ + 1, std::vector<T>(w_ + 1, T{})) {
        for (int i = 0; i < h_; ++i) {
            assert(static_cast<int>(a[i].size()) == w_);
            for (int j = 0; j < w_; ++j)
                prefix_[i + 1][j + 1] = prefix_[i][j + 1] + prefix_[i + 1][j]
                                       - prefix_[i][j] + a[i][j];
        }
    }
    int height() const { return h_; }
    int width() const { return w_; }
    // [top, bottom) x [left, right)
    T sum(int top, int left, int bottom, int right) const {
        assert(0 <= top && top <= bottom && bottom <= h_);
        assert(0 <= left && left <= right && right <= w_);
        return prefix_[bottom][right] - prefix_[top][right]
             - prefix_[bottom][left] + prefix_[top][left];
    }
};

} // namespace ponzoie
