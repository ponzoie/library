#include <functional>
#include <iostream>
#include <vector>

namespace ponzoie {
template <typename T> class SegmentTree {
  private:
    int n;
    vector<T> node;
    // operand
    std::function<T(T, T)> op;
    // 単位元
    T e;

  public:
    SegmentTree(vector<T> vec, function<T(T, T)> op, T e) {
        n = 1;
        while (n < (int)vec.size())
            n <<= 1;
        this->op = op;
        this->e = e;
        // 単位元で初期化
        // nodeは1-indexed
        node.resize(n * 2, e);
        for (int i = 0; i < (int)vec.size(); ++i) {
            node[i + n] = vec[i];
        }
        for (int i = n - 1; i >= 1; --i) {
            node[i] = op(node[i << 1 | 0], node[i << 1 | 1]);
        }
    }

    //[l,r) (0-indexed)
    //[l,r) 区間の和を求める
    T GetNode(int l, int r) {
        T node_l = e, node_r = e;
        l += n;
        r += n;
        while (l < r) {
            if (l & 1)
                node_l = op(node_l, node[l++]);
            if (r & 1)
                node_r = op(node_r, node[--r]);
            l /= 2, r /= 2;
        }
        return op(node_l, node_r);
    }

    // i は 0-indexed
    // vec_i を x へupdate
    void update(int i, T x) {
        i += n;
        node[i] = x;
        while (i > 1) {
            i >>= 1;
            node[i] = op(node[i << 1 | 0], node[i << 1 | 1]);
        }
    }
};
} // namespace ponzoie