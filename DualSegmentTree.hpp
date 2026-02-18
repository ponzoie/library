#include<iostream>
#include<functional>
#include<vector>

namespace ponzoie{
//双対セグメントツリー
// 区間更新、点更新 共に O(log N)

template<typename T>
class DualSegmentTree
{
private:
    int n;
    std::vector<T> node;
    //operand
    //区間に対して行いたい操作
    std::function<T(T,T)> op;
    //単位元
    T e;
public:
    DualSegmentTree(std::vector<T> vec,std::function<T(T,T)> op,T e)
    {
        n = 1;
        while(n < (int)vec.size()) n <<= 1;
        this->op = op;
        this->e = e;
        //単位元で初期化
        //nodeは1-indexed
        node.resize(n*2,e);
        for(int i = 0; i < (int)vec.size(); ++i)
        {
            node[i+n] = vec[i];
        }
    }

    //[l,r) (0-indexed)
    //[l,r) 区間の更新
    //[l,r)区間をxにするわけではない
    //node[i] += x のような挙動
    void update(int l,int r,T x)
    {
        l += n;
        r += n;
        while(l < r)
        {
            if(l & 1)
            {
                node[l] = op(node[l],x);
                ++l;
            } 
            if(r & 1)
            {
                --r;
                node[r] = op(node[r],x);
            }
            l >>= 1, r >>= 1;
            
        }
    }

    // i は 0-indexed
    // vec_i を取得
    T GetNode(int i)
    {
        i += n;
        T x = e;
        while(i != 0)
        {
            x = op(x,node[i]);
            i >>= 1;
        }
        return x;
    }

};
} // namespace shizu;

/*
int op(int a,int b){
    return a+b;
}
int main() {
    int n = 100;
    vector<int> a(n);
    int e = 0;
    for(auto &i:a) i = 20;
    DualSegmentTree<int> dsegt(a,op,e);
    cout<<dsegt.GetNode(20)<<endl;
    dsegt.update(17,30,25);
    for(int i=0;i<100;++i)cout<<dsegt.GetNode(i)<<endl;
}
*/