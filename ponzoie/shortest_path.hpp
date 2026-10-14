#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <limits>
#include <queue>
#include <type_traits>
#include <utility>
#include <vector>

#include "graph.hpp"

namespace ponzoie {

template <class T>
struct shortest_path_result {
    std::vector<T> dist;
    std::vector<int> parent;
    T inf;

    bool reachable(int v) const {
        assert(0 <= v && v < static_cast<int>(dist.size()));
        return dist[v] != inf;
    }

    std::vector<int> path_to(int v) const {
        if (!reachable(v)) return {};
        std::vector<int> path;
        for (; v != -1; v = parent[v]) path.push_back(v);
        std::reverse(path.begin(), path.end());
        return path;
    }
};

inline shortest_path_result<int> bfs(const unweighted_graph& graph, int source) {
    const int n = static_cast<int>(graph.size());
    assert(0 <= source && source < n);
    shortest_path_result<int> result{
        std::vector<int>(n, -1), std::vector<int>(n, -1), -1};
    std::queue<int> que;
    result.dist[source] = 0;
    que.push(source);
    while (!que.empty()) {
        const int v = que.front();
        que.pop();
        for (int to : graph[v]) {
            assert(0 <= to && to < n);
            if (result.dist[to] != -1) continue;
            result.dist[to] = result.dist[v] + 1;
            result.parent[to] = v;
            que.push(to);
        }
    }
    return result;
}

namespace detail {

template <class T>
T distance_add(T a, T b, T inf) {
    if constexpr (std::is_integral_v<T>) {
        const __int128 sum = static_cast<__int128>(a) + b;
        if (sum >= static_cast<__int128>(inf)) return inf;
        if constexpr (std::is_signed_v<T>) {
            if (sum <= -static_cast<__int128>(inf)) return -inf;
        }
        return static_cast<T>(sum);
    } else {
        return std::clamp(a + b, -inf, inf);
    }
}

}  // namespace detail

template <class T>
shortest_path_result<T> dijkstra(
    const weighted_graph<T>& graph, int source,
    T inf = std::numeric_limits<T>::max() / 4) {
    static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
    const int n = static_cast<int>(graph.size());
    assert(0 <= source && source < n && inf > 0);
    shortest_path_result<T> result{
        std::vector<T>(n, inf), std::vector<int>(n, -1), inf};
    using entry = std::pair<T, int>;
    std::priority_queue<entry, std::vector<entry>, std::greater<entry>> que;
    result.dist[source] = 0;
    que.emplace(0, source);
    while (!que.empty()) {
        const auto [distance, v] = que.top();
        que.pop();
        if (distance != result.dist[v]) continue;
        for (const auto& e : graph[v]) {
            assert(0 <= e.to && e.to < n && e.cost >= 0);
            const T next = detail::distance_add(distance, e.cost, inf);
            if (next >= result.dist[e.to]) continue;
            result.dist[e.to] = next;
            result.parent[e.to] = v;
            que.emplace(next, e.to);
        }
    }
    return result;
}

template <class T>
shortest_path_result<T> zero_one_bfs(
    const weighted_graph<T>& graph, int source,
    T inf = std::numeric_limits<T>::max() / 4) {
    static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
    const int n = static_cast<int>(graph.size());
    assert(0 <= source && source < n && inf > 0);
    shortest_path_result<T> result{
        std::vector<T>(n, inf), std::vector<int>(n, -1), inf};
    std::deque<std::pair<T, int>> que;
    result.dist[source] = 0;
    que.emplace_back(0, source);
    while (!que.empty()) {
        const auto [distance, v] = que.front();
        que.pop_front();
        if (distance != result.dist[v]) continue;
        for (const auto& e : graph[v]) {
            assert(0 <= e.to && e.to < n && (e.cost == 0 || e.cost == 1));
            const T next = detail::distance_add(distance, e.cost, inf);
            if (next >= result.dist[e.to]) continue;
            result.dist[e.to] = next;
            result.parent[e.to] = v;
            if (e.cost == 0) que.emplace_front(next, e.to);
            else que.emplace_back(next, e.to);
        }
    }
    return result;
}

template <class T>
struct bellman_ford_result : shortest_path_result<T> {
    // True iff a negative cycle is reachable from the source and can reach v.
    std::vector<bool> negative_cycle;

    bool has_negative_cycle() const {
        return std::find(negative_cycle.begin(), negative_cycle.end(), true) !=
               negative_cycle.end();
    }

    std::vector<int> path_to(int v) const {
        assert(0 <= v && v < static_cast<int>(this->dist.size()));
        if (negative_cycle[v]) return {};
        return shortest_path_result<T>::path_to(v);
    }
};

template <class T>
bellman_ford_result<T> bellman_ford(
    const weighted_graph<T>& graph, int source,
    T inf = std::numeric_limits<T>::max() / 4) {
    static_assert(std::is_arithmetic_v<T> && std::is_signed_v<T>);
    const int n = static_cast<int>(graph.size());
    assert(0 <= source && source < n && inf > 0);
    // A wide working type prevents a repeatedly traversed negative cycle from
    // overflowing T before its affected vertices have been identified.
    using wide = std::conditional_t<std::is_integral_v<T>, __int128, long double>;
    std::vector<wide> dist(n, 0);
    std::vector<bool> reached(n, false);
    bellman_ford_result<T> result;
    result.dist.assign(n, inf);
    result.parent.assign(n, -1);
    result.inf = inf;
    result.negative_cycle.assign(n, false);
    reached[source] = true;
    for (int iteration = 0; iteration < n - 1; ++iteration) {
        bool changed = false;
        for (int v = 0; v < n; ++v) {
            if (!reached[v]) continue;
            for (const auto& e : graph[v]) {
                assert(0 <= e.to && e.to < n);
                const wide next = dist[v] + static_cast<wide>(e.cost);
                if (reached[e.to] && next >= dist[e.to]) continue;
                reached[e.to] = true;
                dist[e.to] = next;
                result.parent[e.to] = v;
                changed = true;
            }
        }
        if (!changed) break;
    }
    std::queue<int> que;
    for (int v = 0; v < n; ++v) {
        if (!reached[v]) continue;
        for (const auto& e : graph[v]) {
            assert(0 <= e.to && e.to < n);
            if ((!reached[e.to] || dist[v] + static_cast<wide>(e.cost) < dist[e.to]) &&
                !result.negative_cycle[e.to]) {
                result.negative_cycle[e.to] = true;
                que.push(e.to);
            }
        }
    }
    while (!que.empty()) {
        const int v = que.front();
        que.pop();
        for (const auto& e : graph[v]) {
            if (result.negative_cycle[e.to]) continue;
            result.negative_cycle[e.to] = true;
            que.push(e.to);
        }
    }
    for (int v = 0; v < n; ++v) {
        if (result.negative_cycle[v]) {
            result.dist[v] = -inf;
            result.parent[v] = -1;
        } else if (reached[v]) {
            assert(-static_cast<wide>(inf) < dist[v] && dist[v] < static_cast<wide>(inf));
            result.dist[v] = static_cast<T>(dist[v]);
        }
    }
    return result;
}

template <class T>
std::vector<std::vector<T>> floyd_warshall(
    const weighted_graph<T>& graph,
    T inf = std::numeric_limits<T>::max() / 4) {
    static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
    assert(inf > 0);
    const int n = static_cast<int>(graph.size());
    std::vector<std::vector<T>> dist(n, std::vector<T>(n, inf));
    for (int v = 0; v < n; ++v) {
        dist[v][v] = 0;
        for (const auto& e : graph[v]) {
            assert(0 <= e.to && e.to < n);
            dist[v][e.to] = std::min(dist[v][e.to], e.cost);
        }
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (dist[i][k] == inf) continue;
            for (int j = 0; j < n; ++j) {
                if (dist[k][j] == inf) continue;
                dist[i][j] = std::min(
                    dist[i][j], detail::distance_add(dist[i][k], dist[k][j], inf));
            }
        }
    }
    return dist;
}

}  // namespace ponzoie
