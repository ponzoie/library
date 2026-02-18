
#include <cassert>
#include <vector>

namespace ponzoie {
// デフォルトの演算
template <class T> T add_default(T a, T b) { return a + b; }
template <class T> T mul_default(T a, T b) { return a * b; }
template <class T> T zero_default() { return T{}; }
template <class T> T one_default() { return T{1}; }
template <class T> T neg_default(T a) { return -a; }

// 代数
//
template <class T, T (*add_op)(T, T) = add_default<T>,
          T (*mul_op)(T, T) = mul_default<T>, T (*zero_op)() = zero_default<T>,
          T (*one_op)() = one_default<T>, T (*neg_op)(T) = neg_default<T>>
struct Ring {
    static constexpr T add(T a, T b) { return add_op(a, b); }
    static constexpr T mul(T a, T b) { return mul_op(a, b); }
    static constexpr T zero() { return zero_op(); }
    static constexpr T one() { return one_op(); }
    static constexpr T neg(T a) { return neg_op(a); }
    static constexpr T sub(S a, S b) { return add(a, neg(b)); }
};

// https://atcoder.jp/contests/abc445/submissions/73408031
template <class T, class Alg = Ring<long long>> struct Matrix {
    std::vector<T> data;
    int h, w;
    Matrix(int h = 0, int w = 0, T fill = Alg::zero())
        : h(h), w(w), data(h * w, fill) {}
    Matrix(int h, int w, std::vector<std::vector<T>> &mat)
        : data(h * w), h(h), w(w) {
        for (int i = 0; i < h; ++i)
            for (int j = 0; j < w; ++j)
                data[i * w + j] = mat[i][j];
    }

    T &at(int i, int j) { return data[i * w + j]; }
    const T &at(int i, int j) const { return data[i * w + j]; }

    Matrix identity_e(int n) const {
        Matrix I(n, n, Alg::zero());
        for (int i = 0; i < n; ++i)
            I.at(i, i) = Alg::one();
        return I;
    }

    Matrix operator+(const Matrix &rhs) const {
        assert(h == rhs.h && w == rhs.w);
        Matrix res(h, w, Alg::zero());
        for (int i = 0; i < h * w; ++i)
            res.data[i] = Alg::add(data[i], rhs.data[i]);
        return res;
    }

    Matrix operator*(const Matrix &rhs) const {
        assert(w == rhs.h);
        Matrix res(h, rhs.w, Alg::zero());
        for (int i = 0; i < h; ++i) {
            for (int k = 0; k < w; ++k) {
                T aik = at(i, k);
                for (int j = 0; j < rhs.w; ++j) {
                    res.at(i, j) =
                        Alg::add(res.at(i, j), Alg::mul(aik, rhs.at(k, j)));
                }
            }
        }
        return res;
    }

    Matrix pow(long long e) const {
        assert(h == w);
        Matrix base = *this;
        Matrix res = this->identity_e(h);
        while (e > 0) {
            if (e & 1)
                res = res * base;
            base = base * base;
            e >>= 1;
        }
        return res;
    }
};
} // namespace ponzoie