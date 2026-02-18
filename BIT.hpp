#include<iostream>
#include<vector>
#include<cassert>
#include<algorithm>
#include<math.h>

namespace ponzoie{
class BIT{
public:
    int n;
    std::vector<long long> a;
    BIT(int n) : n(n),a(n+1,0) {}
    
    //引数にわたす添字は0-indexed
    //a[i]にxを加算
    void add(int i,long long x){
        i++;
        assert(0<=i && i<=n);
        while(i<=n) {
            //nを超えない範囲でLSBを広げてあげる
            a[i] += x;
            i += i & -i;
        }
    }

    //[0,r)までのsumを返す
    long long sum_sub(int r){
        long long ret = 0;
        while(r>0){
            ret += a[r];
            r -= r & -r;
        }
        return ret;
    }

    //[l,r)区間和を求める
    long long sum(int l,int r){
        assert(l<=r && 0<=l && r<=n);
        return sum_sub(r)-sum_sub(l);
    }
};

//座標圧縮
template<typename T>
std::vector<int> ranking(std::vector<T> a){
    std::vector<T> b = a;
    std::ranges::sort(b);
    b.erase(unique(b.begin(),b.end()),b.end());
    std::vector<int> ret(a.size());
    for(int i=0;i<(int)a.size();++i){
        ret[i] = lower_bound(b.begin(),b.end(),a[i]) - b.begin() + 1;
    }
    return ret;
}
}//namespace shizu;

/*
int main() {
    std::vector<int> A = {8, 100, 33, 33, 33, 12, 6, 1211};
    std::vector<int> rank = ranking(A);
    for(auto &i:rank)std::cout<<i<<" ";
    std::cout<<"\n";

    BIT ft(A.size());
    long long ans = 0;
    for(int i=0;i<(int)rank.size();++i){
        ft.add(rank[i]-1,1);
        ans += ft.sum(rank[i],rank.size());
    }
    std::cout<<ans<<"\n";
}

*/