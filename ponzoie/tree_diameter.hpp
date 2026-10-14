#pragma once

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

#include "graph.hpp"
#include "shortest_path.hpp"

namespace ponzoie {

struct tree_diameter_result {
    int from;
    int to;
    int length;
    std::vector<int> path;
};

inline tree_diameter_result tree_diameter(const unweighted_graph& graph) {
    [[maybe_unused]] const int n = static_cast<int>(graph.size());
    assert(n > 0);
    std::size_t adjacency_count = 0;
    for (const auto& adjacent : graph) adjacency_count += adjacent.size();
    assert(adjacency_count == 2 * static_cast<std::size_t>(n - 1));
    const auto first = bfs(graph, 0);
    assert(std::find(first.dist.begin(), first.dist.end(), -1) == first.dist.end());
    const int from = static_cast<int>(
        std::max_element(first.dist.begin(), first.dist.end()) - first.dist.begin());
    const auto second = bfs(graph, from);
    const int to = static_cast<int>(
        std::max_element(second.dist.begin(), second.dist.end()) - second.dist.begin());
    return {from, to, second.dist[to], second.path_to(to)};
}

}  // namespace ponzoie
