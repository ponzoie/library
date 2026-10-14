#ifndef PONZOIE_XOR_BASIS_HPP
#define PONZOIE_XOR_BASIS_HPP

#include <algorithm>
#include <array>
#include <cstdint>

namespace ponzoie {

class xor_basis {
  public:
    // 既存の値の XOR で表せなければ追加して true を返す。
    bool insert(std::uint64_t value) {
        for (int bit = 63; bit >= 0; --bit) {
            if (((value >> bit) & 1U) == 0) continue;
            if (basis_[bit] != 0) value ^= basis_[bit];
            else {
                basis_[bit] = value;
                ++rank_;
                return true;
            }
        }
        return false;
    }

    bool contains(std::uint64_t value) const {
        for (int bit = 63; bit >= 0; --bit)
            if ((value >> bit) & 1U) value ^= basis_[bit];
        return value == 0;
    }

    // seed XOR (基底から選んだ任意の値の XOR) の最大値。
    std::uint64_t max_xor(std::uint64_t seed = 0) const {
        for (int bit = 63; bit >= 0; --bit)
            seed = std::max(seed, seed ^ basis_[bit]);
        return seed;
    }

    int rank() const { return rank_; }
    bool empty() const { return rank_ == 0; }
    void clear() {
        basis_.fill(0);
        rank_ = 0;
    }

  private:
    std::array<std::uint64_t, 64> basis_{};
    int rank_ = 0;
};

} // namespace ponzoie

#endif
