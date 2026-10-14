#pragma once

#include <array>
#include <cassert>
#include <limits>
#include <type_traits>
#include <vector>

namespace ponzoie {

template <class UInt = unsigned long long,
          int Bits = std::numeric_limits<UInt>::digits>
class binary_trie {
    static_assert(std::is_integral_v<UInt> && std::is_unsigned_v<UInt>);
    static_assert(0 < Bits && Bits <= std::numeric_limits<UInt>::digits);
    struct node { std::array<int, 2> next{-1, -1}; int count = 0; };
    std::vector<node> nodes_{1};
    static void check(UInt x) {
        if constexpr (Bits < std::numeric_limits<UInt>::digits) assert((x >> Bits) == 0);
    }
public:
    int size() const { return nodes_[0].count; }
    int count(UInt x) const {
        check(x);
        int v = 0;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            v = nodes_[v].next[(x >> bit) & UInt{1}];
            if (v == -1) return 0;
        }
        return nodes_[v].count;
    }
    void insert(UInt x) {
        check(x);
        int v = 0;
        ++nodes_[v].count;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            int b = static_cast<int>((x >> bit) & UInt{1});
            int to = nodes_[v].next[b];
            if (to == -1) {
                to = static_cast<int>(nodes_.size());
                nodes_[v].next[b] = to;
                nodes_.emplace_back();
            }
            v = to;
            ++nodes_[v].count;
        }
    }
    bool erase(UInt x) {
        if (!count(x)) return false;
        int v = 0;
        --nodes_[v].count;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            v = nodes_[v].next[(x >> bit) & UInt{1}];
            --nodes_[v].count;
        }
        return true;
    }
    // {a XOR x} を昇順にした k 番目（重複込み）。
    UInt kth_xor(int k, UInt x) const {
        check(x);
        assert(0 <= k && k < size());
        int v = 0;
        UInt result = 0;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            int b = static_cast<int>((x >> bit) & UInt{1});
            int to = nodes_[v].next[b];
            int smaller = to == -1 ? 0 : nodes_[to].count;
            if (k < smaller) v = to;
            else {
                k -= smaller;
                result |= UInt{1} << bit;
                v = nodes_[v].next[b ^ 1];
            }
        }
        return result;
    }
    UInt kth(int k) const { return kth_xor(k, UInt{0}); }
    UInt min_xor(UInt x) const { return kth_xor(0, x); }
    UInt max_xor(UInt x) const { return kth_xor(size() - 1, x); }
};

} // namespace ponzoie
