#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")
#pragma GCC target("avx,avx2,fma")
#include <bits/stdc++.h>
using namespace std;
#define all(v) v.begin(),v.end()
#define mod1 1000000007
#define YES cout<<"Yes"<<nline
#define NO cout<<"No"<<nline
#define mod2 998244353
#define ll long long
#define lld long double
#define ull unsigned long long
#define nline "\n"
#define maxbit(n) 31-__builtin_clz(n)
#define maxbitll(n) 63-__builtin_clzll(n)
#define minbit(n) __builtin_ctz(n) 
#define minbitll(n) __builtin_ctzll(n) 
#define bits(n) __builtin_popcount(n)
#define bitsll(n) __builtin_popcountll(n)

template<class Info, class Tag>
struct LazySegmentTree {
    int n;
    std::vector<Info> info;
    std::vector<Tag> tag;
    LazySegmentTree() : n(0) {}
    LazySegmentTree(int n_, Info v_ = Info()) {
        init(n_, v_);
    }
    template<class T>
    LazySegmentTree(std::vector<T> init_) {
        init(init_);
    }
    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size();
        info.assign(4 << std::__lg(n), Info());
        tag.assign(4 << std::__lg(n), Tag());
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init_[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m, r);
            pull(p);
        };
        build(1, 0, n);
    }
    void pull(int p) {
        info[p] = info[2 * p] + info[2 * p + 1];
    }
    void apply(int p, const Tag &v) {
        info[p].apply(v);
        tag[p].apply(v);
    }
    void push(int p) {
        apply(2 * p, tag[p]);
        apply(2 * p + 1, tag[p]);
        tag[p] = Tag();
    }
    void modify(int p, int l, int r, int x, const Info &v) {
        if (r - l == 1) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        if (x < m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p);
    }
    void modify(int p, const Info &v) {
        modify(1, 0, n, p, v);
    }
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) {
            return Info();        }
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        push(p);
        return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m, r, x, y);
    }
    Info rangeQuery(int l, int r) {
        // r+1 for 0 based indexing
        return rangeQuery(1, 0, n, l, r+1);
    }
    
    void rangeApply(int p, int l, int r, int x, int y, const Tag &v) {
        if (l >= y || r <= x) {
            return;
        }
        if (l >= x && r <= y) {
            apply(p, v);
            return;
        }
        int m = (l + r) / 2;
        push(p);
        rangeApply(2 * p, l, m, x, y, v);
        rangeApply(2 * p + 1, m, r, x, y, v);
        pull(p);
    }
    void rangeApply(int l, int r, const Tag &v) {
        // r+1 for 0 based indexing
        return rangeApply(1, 0, n, l, r+1, v);
    }
    // template<class F>
    // vector<int> findnodes(int p, int l, int r, int x, int y) {
    //     if (l >= y || r <= x) {
    //         return {};
    //     }
    //     if (l >= x && r <= y) {
    //         return {p};
    //     }
    //     int m = (l + r) / 2;
    //     push(p);
    //     auto res = findpref(2 * p+1, l, m, x, y, pred,v+info[2*p]);
    //     if (res.first == -1) {
    //         res = findpref(2 * p, m, r, x, y, pred,v);
    //     }
    //     return res;
    // }
    // template<class F>
    // vector<int> findnodes(int l, int r) {
    //     // r+1 for 0 based indexing
    //     return findnodes(1, 0, n, l, r+1);
    // }
    
    template<class F>
    int findFirst(int p, int l, int r, int x, int y, F pred) {
        if (l >= y || r <= x) {
            return -1;
        }
        if (l >= x && r <= y && !pred.apply(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        push(p);
        int res = findFirst(2 * p, l, m, x, y, pred);
        if (res == -1) {
            res = findFirst(2 * p + 1, m, r, x, y, pred);
        }
        return res;
    }
    template<class F>
    int findFirst(int l, int r, F pred) {
        // r+1 for 0 based indexing
        return findFirst(1, 0, n, l, r+1, pred);
    }
    template<class F>
    int findLast(int p, int l, int r, int x, int y, F pred) {
        if (l >= y || r <= x ) {
            return -1;
        }
        if (l >= x && r <= y && !pred.apply(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        push(p);
        int res = findLast(2 * p + 1, m, r, x, y, pred);
        if (res == -1) {
            res = findLast(2 * p, l, m, x, y, pred);
        }
        return res;
    }
    template<class F>
    int findLast(int l, int r, F pred) {
        // r+1 for 0 based indexing
        return findLast(1, 0, n, l, r+1, pred);
    }
};


struct Tag {
    // ll a=1;
    // ll b=0;
    void apply(Tag t) {
        // a*=t.a;
        // b=t.b+b*t.a;
    }
};

struct Info {
    // ll vo=0;
    // ll val=0;
    // ll size=0;
    ll mi=2e18;
    // ll mii=0;
    ll ma=-2e18;
    // ll mai=0;
    // ll m=1;
    // ll supre=0;
    // ll tot=1;
    void apply(Tag t) {
        // val=val*t.a+size*t.b;
        // mi=mi*t.a+t.b;
        // ma=ma*t.a+t.b;
    }
};

Info operator+(const Info &a, const Info &b) {
    Info c;
    // ll md=max(a.m,b.m);
    // c.tot=(a.tot*b.tot)%md;
    // c.supre=(a.supre+a.tot*b.supre)%md;
    // c.m=md;
    // c.vo=a.vo|b.vo;
    // c.val=a.val+b.val;
    // c.size=a.size+b.size;
    c.mi=min(a.mi,b.mi);
    // c.mii=(a.mi<b.mi)?a.mii:b.mii;// <=for left most index
    c.ma=max(a.ma,b.ma);
    // c.mai=(a.ma>b.ma)?a.mai:b.mai;// >= for left most index
    return c;
}

struct pred {
    ll qu=0;
    bool apply(Info t) {
        return t.mi>=qu;
    }
};

void answer(){
    
}

int32_t main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cout << setprecision(30) << fixed;
    int t=1;
    cin>>t;
    for(int i=0;i<t;i++){
       answer();
    }
    return 0;
}