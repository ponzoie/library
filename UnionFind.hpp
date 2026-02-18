#include<vector>
#include<iostream>
#include<cassert>

namespace ponzoie{
class UnionFind {
  private:
    std::vector<int> par;
    std::vector<int> rank;
    std::vector<int> sz;
  
  public:
    UnionFind(int n): par(n,-1), rank(n,0), sz(n,1) {}

    void init(int n) {
      par.resize(n,-1);
      rank.resize(n,0);
      sz.resize(n,1);
    }

    int root(int x) {
      if(par[x] == -1) {
        return x;
      } else {
        //親を見つけるまで再帰
        return par[x] = root(par[x]);
      }
    }

    bool same(int x, int y) {
      return root(x) == root(y);
    }

    bool merge(int x, int y) {
      int rx = root(x);
      int ry = root(y);
      if(rx == ry) {
        return false;
      }

      if(rank[rx] < rank[ry]) {
        std::swap(rx, ry);
      }
      par[ry] = rx;
      if(rank[rx] == rank[ry]) {
        rank[rx]++;
      }
      sz[rx] += sz[ry];
      return true;
    }

    int size(int x) {
      return sz[root(x)];
    }
};
}//namespace shizu