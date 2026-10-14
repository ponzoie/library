#pragma once

#include <cassert>
#include <optional>
#include <queue>
#include <vector>

#include "graph.hpp"

namespace ponzoie {

inline std::optional<std::vector<int>> topological_sort(const unweighted_graph& graph) {
    const int n = static_cast<int>(graph.size());
    std::vector<int> indegree(n, 0);
    for (const auto& adjacent : graph) {
        for (int to : adjacent) {
            assert(0 <= to && to < n);
            ++indegree[to];
        }
    }
    std::queue<int> que;
    for (int v = 0; v < n; ++v) {
        if (indegree[v] == 0) que.push(v);
    }
    std::vector<int> order;
    order.reserve(n);
    while (!que.empty()) {
        const int v = que.front();
        que.pop();
        order.push_back(v);
        for (int to : graph[v]) {
            if (--indegree[to] == 0) que.push(to);
        }
    }
    if (static_cast<int>(order.size()) != n) return std::nullopt;
    return order;
}

}  // namespace ponzoie
