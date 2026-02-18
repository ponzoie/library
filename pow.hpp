#include <cassert>
#include <iostream>
namespace ponzoie {
long long powll(long long a, long long x) {
    if (x == 0)
        return 1LL;
    long long ret = 1LL;
    while (x > 0) {
        if (x & 1)
            ret *= a;
        a *= a;
        x >>= 1;
    }
    return ret;
}

long long modpow(long long a, long long n, long long m) {
    assert(m >= 1);
    long long res = 1;
    while (n > 0) {
        if (n & 1)
            res = res * a % m;
        a = a * a % m;
        n >>= 1;
    }
    return res;
}
} // namespace ponzoie