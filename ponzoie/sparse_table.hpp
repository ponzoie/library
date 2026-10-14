#pragma once

#include <cassert>
#include <utility>
#include <vector>

namespace ponzoie {

// op must be associative and idempotent: op(x, x) == x.
template <class T, auto op>
class sparse_table {
public:
    sparse_table() = default;

    explicit sparse_table(const std::vector<T>& values)
        : n_(static_cast<int>(values.size())), log_(n_ + 1, 0) {
        for (int i = 2; i <= n_; ++i) log_[i] = log_[i / 2] + 1;
        if (n_ == 0) return;
        table_.push_back(values);
        for (int k = 1; k <= log_[n_]; ++k) {
            const int width = 1 << k;
            const int half = width >> 1;
            std::vector<T> row;
            row.reserve(n_ - width + 1);
            for (int i = 0; i + width <= n_; ++i) {
                row.push_back(op(table_[k - 1][i], table_[k - 1][i + half]));
            }
            table_.push_back(std::move(row));
        }
    }

    int size() const { return n_; }

    T prod(int l, int r) const {
        assert(0 <= l && l < r && r <= n_);
        const int k = log_[r - l];
        return op(table_[k][l], table_[k][r - (1 << k)]);
    }

    T query(int l, int r) const { return prod(l, r); }

private:
    int n_ = 0;
    std::vector<int> log_;
    std::vector<std::vector<T>> table_;
};

}  // namespace ponzoie
