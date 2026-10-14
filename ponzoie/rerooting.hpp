#pragma once

#include <cassert>
#include <vector>

namespace ponzoie {

// merge は結合的、e は単位元。
// put_vertex(集約値, 頂点)、put_edge(頂点DP, from, to)。
template <class T, auto merge, auto e, auto put_vertex, auto put_edge>
std::vector<T> rerooting(const std::vector<std::vector<int>>& graph) {
    int n = static_cast<int>(graph.size());
    if (n == 0) return {};
    std::vector<int> parent(n, -2), order{0};
    parent[0] = -1;
    for (int i = 0; i < static_cast<int>(order.size()); ++i) {
        int v = order[i], parent_edges = 0;
        for (int u : graph[v]) {
            assert(0 <= u && u < n);
            if (u == parent[v]) { ++parent_edges; continue; }
            assert(parent[u] == -2);
            parent[u] = v;
            order.push_back(u);
        }
        assert(parent_edges == (v == 0 ? 0 : 1));
    }
    assert(static_cast<int>(order.size()) == n);
    std::vector<T> down(n, e()), up(n, e()), answer(n, e());
    for (int i = n - 1; i >= 0; --i) {
        int v = order[i];
        T accum = e();
        for (int u : graph[v]) if (u != parent[v])
            accum = merge(accum, put_edge(down[u], u, v));
        down[v] = put_vertex(accum, v);
    }
    for (int v : order) {
        int d = static_cast<int>(graph[v].size());
        std::vector<T> contributions(d, e()), prefix(d + 1, e()), suffix(d + 1, e());
        for (int i = 0; i < d; ++i) {
            int u = graph[v][i];
            contributions[i] = u == parent[v] ? up[v] : put_edge(down[u], u, v);
            prefix[i + 1] = merge(prefix[i], contributions[i]);
        }
        for (int i = d - 1; i >= 0; --i) suffix[i] = merge(contributions[i], suffix[i + 1]);
        answer[v] = put_vertex(prefix[d], v);
        for (int i = 0; i < d; ++i) {
            int u = graph[v][i];
            if (u == parent[v]) continue;
            T without = merge(prefix[i], suffix[i + 1]);
            up[u] = put_edge(put_vertex(without, v), v, u);
        }
    }
    return answer;
}

} // namespace ponzoie
