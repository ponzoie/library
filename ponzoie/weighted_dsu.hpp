#pragma once

#include <cassert>
#include <utility>
#include <vector>

namespace ponzoie {

// Maintains constraints potential(y) - potential(x) = w.
template <class T>
class weighted_dsu {
public:
    explicit weighted_dsu(int n = 0) : n_(n) {
        assert(n >= 0);
        parent_or_size_.assign(n, -1);
        weight_to_parent_.assign(n, T{});
    }

    int size() const { return n_; }

    int leader(int a) {
        assert(0 <= a && a < n_);
        if (parent_or_size_[a] < 0) return a;
        const int parent = parent_or_size_[a];
        const int root = leader(parent);
        weight_to_parent_[a] = weight_to_parent_[a] + weight_to_parent_[parent];
        return parent_or_size_[a] = root;
    }

    bool same(int a, int b) { return leader(a) == leader(b); }
    int size(int a) { return -parent_or_size_[leader(a)]; }

    // Potential relative to the current component leader.
    T potential(int a) {
        leader(a);
        return weight_to_parent_[a];
    }

    T diff(int x, int y) {
        assert(same(x, y));
        return potential(y) - potential(x);
    }

    // True for a new or already satisfied constraint; false for a contradiction.
    bool merge(int x, int y, T w) {
        int root_x = leader(x), root_y = leader(y);
        w = w + weight_to_parent_[x] - weight_to_parent_[y];
        if (root_x == root_y) return w == T{};
        if (-parent_or_size_[root_x] < -parent_or_size_[root_y]) {
            std::swap(root_x, root_y);
            w = T{} - w;
        }
        parent_or_size_[root_x] += parent_or_size_[root_y];
        parent_or_size_[root_y] = root_x;
        weight_to_parent_[root_y] = w;
        return true;
    }

private:
    int n_;
    std::vector<int> parent_or_size_;
    std::vector<T> weight_to_parent_;
};

}  // namespace ponzoie
