#pragma once

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace ponzoie {

template <class T>
class coordinate_compression {
public:
    coordinate_compression() = default;

    explicit coordinate_compression(std::vector<T> values)
        : values_(std::move(values)) {
        std::sort(values_.begin(), values_.end());
        values_.erase(std::unique(values_.begin(), values_.end()), values_.end());
    }

    int size() const { return static_cast<int>(values_.size()); }

    int index(const T& x) const {
        const int i = lower_bound(x);
        assert(i < size() && values_[i] == x);
        return i;
    }

    int lower_bound(const T& x) const {
        return static_cast<int>(std::lower_bound(values_.begin(), values_.end(), x)
                                - values_.begin());
    }

    int upper_bound(const T& x) const {
        return static_cast<int>(std::upper_bound(values_.begin(), values_.end(), x)
                                - values_.begin());
    }

    T value(int i) const {
        assert(0 <= i && i < size());
        return values_[i];
    }

    const std::vector<T>& values() const { return values_; }

private:
    std::vector<T> values_;
};

template <class T>
std::vector<int> compress(const std::vector<T>& values) {
    const coordinate_compression<T> coordinates(values);
    std::vector<int> result;
    result.reserve(values.size());
    for (const T& x : values) result.push_back(coordinates.index(x));
    return result;
}

}  // namespace ponzoie
