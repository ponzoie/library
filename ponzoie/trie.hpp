#pragma once

#include <array>
#include <cassert>
#include <string_view>
#include <vector>

namespace ponzoie {

template <int Alphabet = 26, char Base = 'a'> class trie {
    static_assert(Alphabet > 0);
    struct node {
        std::array<int, Alphabet> next;
        int pass = 0, terminal = 0;
        node() { next.fill(-1); }
    };
    std::vector<node> nodes_{1};
    static int index(char c) {
        int x = static_cast<unsigned char>(c) - static_cast<unsigned char>(Base);
        assert(0 <= x && x < Alphabet);
        return x;
    }
    int find(std::string_view s) const {
        int v = 0;
        for (char c : s) {
            v = nodes_[v].next[index(c)];
            if (v == -1) return -1;
        }
        return v;
    }
public:
    int size() const { return nodes_[0].pass; }
    int node_count() const { return static_cast<int>(nodes_.size()); }
    void insert(std::string_view s) {
        int v = 0;
        ++nodes_[v].pass;
        for (char c : s) {
            int x = index(c), to = nodes_[v].next[x];
            if (to == -1) {
                to = static_cast<int>(nodes_.size());
                nodes_[v].next[x] = to;
                nodes_.emplace_back();
            }
            v = to;
            ++nodes_[v].pass;
        }
        ++nodes_[v].terminal;
    }
    int count(std::string_view s) const {
        int v = find(s);
        return v == -1 ? 0 : nodes_[v].terminal;
    }
    int count_prefix(std::string_view s) const {
        int v = find(s);
        return v == -1 ? 0 : nodes_[v].pass;
    }
    bool contains(std::string_view s) const { return count(s) > 0; }
    // 同じ文字列を1個消す。存在しなければ false。
    bool erase(std::string_view s) {
        if (!contains(s)) return false;
        int v = 0;
        --nodes_[v].pass;
        for (char c : s) { v = nodes_[v].next[index(c)]; --nodes_[v].pass; }
        --nodes_[v].terminal;
        return true;
    }
};

} // namespace ponzoie
