#ifndef PONZOIE_MATRIX_HPP
#define PONZOIE_MATRIX_HPP

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <vector>

namespace ponzoie {

template <class T> constexpr T add_default(T a, T b) { return a + b; }
template <class T> constexpr T mul_default(T a, T b) { return a * b; }
template <class T> constexpr T zero_default() { return T{}; }
template <class T> constexpr T one_default() { return T{1}; }
template <class T> constexpr T neg_default(T a) { return -a; }

// add/mul/zero/one だけ定義した半環も Matrix の乗算・累乗に利用できる。
template <class T, T (*add_op)(T, T) = add_default<T>,
          T (*mul_op)(T, T) = mul_default<T>, T (*zero_op)() = zero_default<T>,
          T (*one_op)() = one_default<T>, T (*neg_op)(T) = neg_default<T>>
struct Ring {
    static constexpr T add(T a, T b) { return add_op(a, b); }
    static constexpr T mul(T a, T b) { return mul_op(a, b); }
    static constexpr T zero() { return zero_op(); }
    static constexpr T one() { return one_op(); }
    static constexpr T neg(T a) { return neg_op(a); }
    static constexpr T sub(T a, T b) { return add(a, neg(b)); }
};

template <class T, class Alg = Ring<T>> struct Matrix {
    using reference = typename std::vector<T>::reference;
    using const_reference = typename std::vector<T>::const_reference;

    // 旧 math.hpp との互換用。形を変えるときは新しい Matrix を構築する。
    std::vector<T> data;
    int h = 0, w = 0;

    Matrix(int height = 0, int width = 0, T fill = Alg::zero())
        : data(storage_size(height, width), fill), h(height), w(width) {}

    explicit Matrix(const std::vector<std::vector<T>>& rows)
        : Matrix(row_count(rows.size()),
                 rows.empty() ? 0 : row_count(rows.front().size())) {
        for (int i = 0; i < h; ++i) {
            assert(rows[i].size() == static_cast<std::size_t>(w));
            for (int j = 0; j < w; ++j) at(i, j) = rows[i][j];
        }
    }

    Matrix(std::initializer_list<std::initializer_list<T>> rows)
        : Matrix(row_count(rows.size()),
                 rows.size() == 0 ? 0 : row_count(rows.begin()->size())) {
        int i = 0;
        for (const auto& row : rows) {
            assert(row.size() == static_cast<std::size_t>(w));
            int j = 0;
            for (const auto& value : row) at(i, j++) = value;
            ++i;
        }
    }

    Matrix(int height, int width, const std::vector<std::vector<T>>& rows)
        : Matrix(rows) {
        assert(h == height && w == width);
    }

    int height() const { return h; }
    int width() const { return w; }

    reference at(int i, int j) {
        assert(0 <= i && i < h && 0 <= j && j < w);
        return data[static_cast<std::size_t>(i) * w + j];
    }
    const_reference at(int i, int j) const {
        assert(0 <= i && i < h && 0 <= j && j < w);
        return data[static_cast<std::size_t>(i) * w + j];
    }
    reference operator()(int i, int j) { return at(i, j); }
    const_reference operator()(int i, int j) const { return at(i, j); }

    static Matrix identity(int n) {
        Matrix result(n, n);
        for (int i = 0; i < n; ++i) result(i, i) = Alg::one();
        return result;
    }
    Matrix identity_e(int n) const { return identity(n); }

    Matrix& operator+=(const Matrix& rhs) {
        assert(h == rhs.h && w == rhs.w);
        for (std::size_t i = 0; i < data.size(); ++i)
            data[i] = Alg::add(data[i], rhs.data[i]);
        return *this;
    }
    Matrix& operator-=(const Matrix& rhs) {
        assert(h == rhs.h && w == rhs.w);
        for (std::size_t i = 0; i < data.size(); ++i)
            data[i] = Alg::sub(data[i], rhs.data[i]);
        return *this;
    }
    Matrix& operator*=(const Matrix& rhs) { return *this = *this * rhs; }
    friend Matrix operator+(Matrix lhs, const Matrix& rhs) { return lhs += rhs; }
    friend Matrix operator-(Matrix lhs, const Matrix& rhs) { return lhs -= rhs; }

    Matrix operator*(const Matrix& rhs) const {
        assert(w == rhs.h);
        Matrix result(h, rhs.w);
        for (int i = 0; i < h; ++i)
            for (int k = 0; k < w; ++k)
                for (int j = 0; j < rhs.w; ++j)
                    result(i, j) = Alg::add(result(i, j),
                                           Alg::mul(at(i, k), rhs(k, j)));
        return result;
    }

    // 列ベクトル v に対して A v を返す。
    std::vector<T> apply(const std::vector<T>& v) const {
        assert(v.size() == static_cast<std::size_t>(w));
        std::vector<T> result(h, Alg::zero());
        for (int i = 0; i < h; ++i)
            for (int j = 0; j < w; ++j)
                result[i] = Alg::add(result[i], Alg::mul(at(i, j), v[j]));
        return result;
    }

    Matrix pow(long long exponent) const {
        assert(h == w && exponent >= 0);
        Matrix base = *this, result = identity(h);
        while (exponent > 0) {
            if (exponent & 1) result *= base;
            exponent >>= 1;
            if (exponent > 0) base *= base;
        }
        return result;
    }

    friend bool operator==(const Matrix& lhs, const Matrix& rhs) {
        return lhs.h == rhs.h && lhs.w == rhs.w && lhs.data == rhs.data;
    }

  private:
    static int row_count(std::size_t n) {
        assert(n <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
        return static_cast<int>(n);
    }
    static std::size_t storage_size(int height, int width) {
        assert(height >= 0 && width >= 0);
        const auto h = static_cast<std::size_t>(height);
        const auto w = static_cast<std::size_t>(width);
        assert(w == 0 || h <= std::numeric_limits<std::size_t>::max() / w);
        return h * w;
    }
};

} // namespace ponzoie

#endif
