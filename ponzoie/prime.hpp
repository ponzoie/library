#ifndef PONZOIE_PRIME_HPP
#define PONZOIE_PRIME_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <utility>
#include <vector>

namespace ponzoie {

// 正の整数の素因数分解。sqrt(n) 程度まで試し割りする。
inline std::vector<std::pair<long long, int>> factorize(long long n) {
    assert(n >= 1);
    std::vector<std::pair<long long, int>> result;
    for (long long p = 2; p <= n / p; p += (p == 2 ? 1 : 2)) {
        if (n % p != 0) continue;
        int count = 0;
        do {
            n /= p;
            ++count;
        } while (n % p == 0);
        result.emplace_back(p, count);
    }
    if (n > 1) result.emplace_back(n, 1);
    return result;
}

namespace internal {
template <class Int>
std::vector<Int> divisors_from_factorization(
    const std::vector<std::pair<Int, int>>& factors) {
    std::vector<Int> result{1};
    for (auto [p, exponent] : factors) {
        const auto old_size = result.size();
        Int power = 1;
        for (int e = 1; e <= exponent; ++e) {
            power *= p;
            for (std::size_t i = 0; i < old_size; ++i)
                result.push_back(result[i] * power);
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}
} // namespace internal

inline std::vector<long long> divisors(long long n) {
    return internal::divisors_from_factorization(factorize(n));
}

inline long long totient(long long n) {
    long long result = n;
    for (auto [p, exponent] : factorize(n)) {
        (void)exponent;
        result = result / p * (p - 1);
    }
    return result;
}

// 線形篩。多数の小さい整数を分解する場合に利用する。
class prime_table {
  public:
    explicit prime_table(int n) : limit_(n), spf_(table_size(n), 0) {
        for (int i = 2; i <= n; ++i) {
            if (spf_[i] == 0) {
                spf_[i] = i;
                primes_.push_back(i);
            }
            for (int p : primes_) {
                if (p > spf_[i] || p > n / i) break;
                spf_[i * p] = p;
            }
        }
    }

    int limit() const { return limit_; }
    const std::vector<int>& primes() const { return primes_; }
    bool is_prime(int n) const {
        assert(0 <= n && n <= limit_);
        return n >= 2 && spf_[n] == n;
    }
    int smallest_prime_factor(int n) const {
        assert(2 <= n && n <= limit_);
        return spf_[n];
    }

    std::vector<std::pair<int, int>> factorize(int n) const {
        assert(1 <= n && n <= limit_);
        std::vector<std::pair<int, int>> result;
        while (n > 1) {
            const int p = spf_[n];
            int count = 0;
            do {
                n /= p;
                ++count;
            } while (n > 1 && spf_[n] == p);
            result.emplace_back(p, count);
        }
        return result;
    }
    std::vector<int> divisors(int n) const {
        return internal::divisors_from_factorization(factorize(n));
    }
    int totient(int n) const {
        int result = n;
        for (auto [p, exponent] : factorize(n)) {
            (void)exponent;
            result = result / p * (p - 1);
        }
        return result;
    }

  private:
    int limit_;
    std::vector<int> spf_, primes_;
    static std::size_t table_size(int n) {
        assert(0 <= n && n < std::numeric_limits<int>::max());
        return static_cast<std::size_t>(n) + 1;
    }
};

} // namespace ponzoie

#endif
