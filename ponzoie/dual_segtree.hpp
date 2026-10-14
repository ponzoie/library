#pragma once

#include <algorithm>
#include <cassert>
#include <optional>
#include <vector>

namespace ponzoie {

// composition(f, g) applies g first, then f.
template <class S, class F, auto mapping, auto composition, auto id>
class dual_segtree {
public:
    using value_type = S;
    using function_type = F;

    explicit dual_segtree(int n = 0, S initial = S{}) : n_(n) {
        assert(n >= 0);
        initialize(initial);
    }

    explicit dual_segtree(const std::vector<S>& values)
        : n_(static_cast<int>(values.size())) {
        initialize(S{});
        std::copy(values.begin(), values.end(), data_.begin());
    }

    int size() const { return n_; }

    void set(int p, S value) {
        assert(0 <= p && p < n_);
        push_to(p + capacity_);
        data_[p] = value;
    }

    S get(int p) {
        assert(0 <= p && p < n_);
        push_to(p + capacity_);
        return data_[p];
    }

    void apply(int p, F f) {
        assert(0 <= p && p < n_);
        push_to(p + capacity_);
        data_[p] = mapping(f, data_[p]);
    }

    // Applies f to every element of the half-open interval [l, r).
    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= n_);
        if (l == r) return;
        l += capacity_;
        r += capacity_;
        // An older ancestor update must reach a child before a newer update.
        push_to(l);
        push_to(r - 1);
        while (l < r) {
            if (l & 1) all_apply(l++, f);
            if (r & 1) all_apply(--r, f);
            l >>= 1;
            r >>= 1;
        }
    }

private:
    int n_;
    int capacity_ = 1;
    int log_ = 0;
    std::vector<S> data_;
    std::vector<F> lazy_;

    void initialize(S initial) {
        while (capacity_ < n_) {
            capacity_ <<= 1;
            ++log_;
        }
        data_.assign(capacity_, initial);
        lazy_.assign(capacity_, id());
    }

    void all_apply(int k, const F& f) {
        if (k < capacity_) {
            lazy_[k] = composition(f, lazy_[k]);
        } else {
            data_[k - capacity_] = mapping(f, data_[k - capacity_]);
        }
    }

    void push(int k) {
        all_apply(k << 1, lazy_[k]);
        all_apply(k << 1 | 1, lazy_[k]);
        lazy_[k] = id();
    }

    void push_to(int p) {
        for (int i = log_; i >= 1; --i) push(p >> i);
    }
};

namespace internal {
template <class T>
T dual_add_mapping(T f, T x) { return x + f; }
template <class T>
T dual_add_composition(T f, T g) { return f + g; }
template <class T>
T dual_add_id() { return T{}; }

template <class T>
T dual_assign_mapping(const std::optional<T>& f, const T& x) {
    return f ? *f : x;
}
template <class T>
std::optional<T> dual_assign_composition(const std::optional<T>& f,
                                        const std::optional<T>& g) {
    return f ? f : g;
}
template <class T>
std::optional<T> dual_assign_id() { return std::nullopt; }
}  // namespace internal

template <class T>
using range_add_point_get =
    dual_segtree<T, T, internal::dual_add_mapping<T>,
                 internal::dual_add_composition<T>, internal::dual_add_id<T>>;

template <class T>
using range_assign_point_get =
    dual_segtree<T, std::optional<T>, internal::dual_assign_mapping<T>,
                 internal::dual_assign_composition<T>, internal::dual_assign_id<T>>;

template <class T>
struct affine_function {
    T a = T{1};
    T b = T{};
};

namespace internal {
template <class T>
T dual_affine_mapping(affine_function<T> f, T x) {
    return f.a * x + f.b;
}
template <class T>
affine_function<T> dual_affine_composition(affine_function<T> f,
                                         affine_function<T> g) {
    return {f.a * g.a, f.a * g.b + f.b};
}
template <class T>
affine_function<T> dual_affine_id() { return {}; }
}  // namespace internal

template <class T>
using range_affine_point_get =
    dual_segtree<T, affine_function<T>, internal::dual_affine_mapping<T>,
                 internal::dual_affine_composition<T>, internal::dual_affine_id<T>>;

}  // namespace ponzoie
