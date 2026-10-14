#pragma once

#include <vector>

namespace ponzoie {

template <class T = long long>
struct edge {
    int to;
    T cost;
};

template <class T = long long>
using weighted_graph = std::vector<std::vector<edge<T>>>;

using unweighted_graph = std::vector<std::vector<int>>;

}  // namespace ponzoie
