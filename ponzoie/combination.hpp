#ifndef PONZOIE_COMBINATION_HPP
#define PONZOIE_COMBINATION_HPP

#include <cassert>
#include <cstddef>
#include <vector>

namespace ponzoie {

// Mint は ACL の static_modint / dynamic_modint など。
// 法は素数、前計算する n は法未満でなければならない。
template <class Mint> class combination {
  public:
    explicit combination(int n = 0)
        : modulus_(Mint::mod()), factorial_(1, Mint(1)), inverse_factorial_(1, Mint(1)) {
        extend(n);
    }

    int limit() const { return static_cast<int>(factorial_.size()) - 1; }

    void extend(int n) {
        check_modulus();
        assert(0 <= n && n < modulus_);
        const int old_limit = limit();
        if (n <= old_limit) return;
        factorial_.resize(static_cast<std::size_t>(n) + 1);
        inverse_factorial_.resize(static_cast<std::size_t>(n) + 1);
        for (int i = old_limit + 1; i <= n; ++i)
            factorial_[i] = factorial_[i - 1] * Mint(i);
        inverse_factorial_[n] = factorial_[n].inv();
        for (int i = n; i > old_limit; --i)
            inverse_factorial_[i - 1] = inverse_factorial_[i] * Mint(i);
    }

    Mint factorial(int n) const {
        check_modulus();
        assert(0 <= n && n <= limit());
        return factorial_[n];
    }
    Mint inverse_factorial(int n) const {
        check_modulus();
        assert(0 <= n && n <= limit());
        return inverse_factorial_[n];
    }

    Mint C(long long n, long long k) const {
        check_modulus();
        if (n < 0 || k < 0 || k > n) return Mint(0);
        assert(n <= limit());
        return factorial_[n] * inverse_factorial_[k] * inverse_factorial_[n - k];
    }
    Mint P(long long n, long long k) const {
        check_modulus();
        if (n < 0 || k < 0 || k > n) return Mint(0);
        assert(n <= limit());
        return factorial_[n] * inverse_factorial_[n - k];
    }
    Mint multichoose(long long n, long long k) const {
        check_modulus();
        if (n < 0 || k < 0) return Mint(0);
        if (k == 0) return Mint(1);
        if (n == 0) return Mint(0);
        assert(k <= limit() && n <= static_cast<long long>(limit()) + 1 - k);
        return C(n + k - 1, k);
    }

  private:
    int modulus_;
    std::vector<Mint> factorial_, inverse_factorial_;
    void check_modulus() const { assert(Mint::mod() == modulus_); }
};

} // namespace ponzoie

#endif
