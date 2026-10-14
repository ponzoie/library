#pragma once

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace ponzoie {

class lowlink {
public:
    struct undirected_edge { int from; int to; };

    explicit lowlink(int n = 0) : n_(n), graph_(n) { assert(n >= 0); }

    int size() const { return n_; }
    int edge_count() const { return static_cast<int>(edges_.size()); }

    int add_edge(int u, int v) {
        assert(0 <= u && u < n_ && 0 <= v && v < n_);
        const int id = edge_count();
        edges_.push_back({u, v});
        graph_[u].emplace_back(v, id);
        graph_[v].emplace_back(u, id);
        built_ = false;
        return id;
    }

    const std::vector<undirected_edge>& edges() const { return edges_; }

    void build() {
        order_.assign(n_, -1);
        low_.assign(n_, -1);
        bridges_.clear();
        articulation_points_.clear();
        std::vector<int> parent(n_, -1), parent_edge(n_, -1), children(n_, 0);
        std::vector<bool> articulation(n_, false);
        std::vector<bool> bridge(edges_.size(), false);
        struct frame { int v; std::size_t next; };
        std::vector<frame> stack;
        stack.reserve(n_);
        int timer = 0;
        for (int root = 0; root < n_; ++root) {
            if (order_[root] != -1) continue;
            order_[root] = low_[root] = timer++;
            stack.push_back({root, 0});
            while (!stack.empty()) {
                auto& current = stack.back();
                const int v = current.v;
                if (current.next == graph_[v].size()) {
                    stack.pop_back();
                    const int p = parent[v];
                    if (p == -1) {
                        articulation[v] = children[v] >= 2;
                    } else {
                        low_[p] = std::min(low_[p], low_[v]);
                        if (order_[p] < low_[v]) bridge[parent_edge[v]] = true;
                        if (parent[p] != -1 && order_[p] <= low_[v]) articulation[p] = true;
                    }
                    continue;
                }
                const auto [to, id] = graph_[v][current.next++];
                if (id == parent_edge[v]) continue;
                if (order_[to] != -1) {
                    low_[v] = std::min(low_[v], order_[to]);
                    continue;
                }
                parent[to] = v;
                parent_edge[to] = id;
                ++children[v];
                order_[to] = low_[to] = timer++;
                stack.push_back({to, 0});
            }
        }
        for (int id = 0; id < edge_count(); ++id) {
            if (bridge[id]) bridges_.push_back(id);
        }
        for (int v = 0; v < n_; ++v) {
            if (articulation[v]) articulation_points_.push_back(v);
        }
        built_ = true;
    }

    const std::vector<int>& order() const { assert(built_); return order_; }
    const std::vector<int>& low() const { assert(built_); return low_; }
    const std::vector<int>& bridges() const { assert(built_); return bridges_; }
    const std::vector<int>& articulation_points() const {
        assert(built_);
        return articulation_points_;
    }

private:
    int n_;
    bool built_ = false;
    std::vector<undirected_edge> edges_;
    std::vector<std::vector<std::pair<int, int>>> graph_;
    std::vector<int> order_, low_, bridges_, articulation_points_;
};

}  // namespace ponzoie
