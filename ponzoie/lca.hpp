#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>

#include "graph.hpp"

namespace ponzoie {

class lca_tree {
public:
    lca_tree() = default;

    explicit lca_tree(const unweighted_graph& graph, int root = 0) {
        build(graph, root);
    }

    void build(const unweighted_graph& graph, int root = 0) {
        n_ = static_cast<int>(graph.size());
        assert(0 <= root && root < n_);
        root_ = root;
        depth_.assign(n_, -1);
        parent_.assign(n_, -1);
        in_.assign(n_, 0);
        out_.assign(n_, 0);
        struct frame { int v; std::size_t next; };
        std::vector<frame> stack;
        stack.reserve(n_);
        depth_[root] = 0;
        int timer = 0;
        in_[root] = timer++;
        stack.push_back({root, 0});
        std::size_t adjacency_count = 0;
        for (const auto& adjacent : graph) adjacency_count += adjacent.size();
        assert(adjacency_count == 2 * static_cast<std::size_t>(n_ - 1));
        while (!stack.empty()) {
            auto& current = stack.back();
            const int v = current.v;
            if (current.next == graph[v].size()) {
                out_[v] = timer;
                stack.pop_back();
                continue;
            }
            const int to = graph[v][current.next++];
            assert(0 <= to && to < n_);
            if (to == parent_[v]) continue;
            assert(depth_[to] == -1);
            parent_[to] = v;
            depth_[to] = depth_[v] + 1;
            in_[to] = timer++;
            stack.push_back({to, 0});
        }
        assert(timer == n_);
        int levels = 1;
        while ((std::uint64_t{1} << levels) <= static_cast<std::uint64_t>(n_)) ++levels;
        up_.assign(levels, std::vector<int>(n_, root));
        for (int v = 0; v < n_; ++v) {
            up_[0][v] = parent_[v] == -1 ? root : parent_[v];
        }
        for (int k = 1; k < levels; ++k) {
            for (int v = 0; v < n_; ++v) up_[k][v] = up_[k - 1][up_[k - 1][v]];
        }
    }

    int size() const { return n_; }
    int root() const { return root_; }

    int depth(int v) const { check_vertex(v); return depth_[v]; }
    int parent(int v) const { check_vertex(v); return parent_[v]; }
    int entry(int v) const { check_vertex(v); return in_[v]; }

    std::pair<int, int> subtree_interval(int v) const {
        check_vertex(v);
        return {in_[v], out_[v]};
    }

    bool is_ancestor(int ancestor, int v) const {
        check_vertex(ancestor);
        check_vertex(v);
        return in_[ancestor] <= in_[v] && in_[v] < out_[ancestor];
    }

    // Returns -1 if k exceeds the distance to the root.
    int kth_ancestor(int v, long long k) const {
        check_vertex(v);
        assert(k >= 0);
        if (k > depth_[v]) return -1;
        for (int bit = 0; k != 0; ++bit, k >>= 1) {
            if (k & 1) v = up_[bit][v];
        }
        return v;
    }

    int lca(int u, int v) const {
        check_vertex(u);
        check_vertex(v);
        if (depth_[u] < depth_[v]) std::swap(u, v);
        u = kth_ancestor(u, depth_[u] - depth_[v]);
        if (u == v) return u;
        for (int k = static_cast<int>(up_.size()) - 1; k >= 0; --k) {
            if (up_[k][u] == up_[k][v]) continue;
            u = up_[k][u];
            v = up_[k][v];
        }
        return up_[0][u];
    }

    int distance(int u, int v) const {
        const int ancestor = lca(u, v);
        return (depth_[u] - depth_[ancestor]) + (depth_[v] - depth_[ancestor]);
    }

    // Vertex k edges along the path u -> v, or -1 if k is too large.
    int jump(int u, int v, long long k) const {
        assert(k >= 0);
        const int ancestor = lca(u, v);
        const int left = depth_[u] - depth_[ancestor];
        const int right = depth_[v] - depth_[ancestor];
        if (k > static_cast<long long>(left) + right) return -1;
        if (k <= left) return kth_ancestor(u, k);
        return kth_ancestor(v, static_cast<long long>(left) + right - k);
    }

private:
    int n_ = 0;
    int root_ = -1;
    std::vector<int> depth_, parent_, in_, out_;
    std::vector<std::vector<int>> up_;

    void check_vertex([[maybe_unused]] int v) const { assert(0 <= v && v < n_); }
};

}  // namespace ponzoie
