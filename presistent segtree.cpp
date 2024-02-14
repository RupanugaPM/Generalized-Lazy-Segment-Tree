#include<bits/stdc++.h>
using namespace std;

struct Vertex {
    Vertex *l, *r;
    long long int sum;
    long long int size;

    Vertex(int val) : l(nullptr), r(nullptr), sum(val),size(1) {}
    Vertex(Vertex *l, Vertex *r) : l(l), r(r), sum(0),size(0) {
        if (l){sum += l->sum;size+=l->size;}
        if (r) {sum += r->sum;size+=r->size;}
    }
};

Vertex* build(vector<pair<long long int,long long int>>&a, int tl, int tr) {
    if (tl == tr)
        return new Vertex(a[tl].second);
    int tm = (tl + tr) / 2;
    return new Vertex(build(a, tl, tm), build(a, tm+1, tr));
}

long long int get_point(Vertex*v,long long int x){
    if(!v->l && !v->r){
        return v->sum;
    }
    if(v->l){
        if(v->l->size>=x){
            return get_point(v->l,x);
        }
        return get_point(v->r,x-v->l->size);
    }
    return get_point(v->r,x);
}

void print(Vertex* v){
    if(!v) return;
    print(v->l);
    if(v->size==1){
        cout<<v->sum<<" ";
    }
    print(v->r);
}


long long int get_sum(Vertex* v, long long int tl,long long  int tr,long long int l,long long int r) {
    if (l > r)
        return 0LL;
    if (l == tl && tr == r)
        return v->sum;
    long long int tm = (v->l)?v->l->size:0LL;
    tm+=tl-1LL;
    return get_sum(v->l, tl, tm, l, min(r, tm))
         + get_sum(v->r, tm+1, tr, max(l, tm+1), r);
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cout << setprecision(30) << fixed;
    long long int t;
    cin>>t;
    for(int ii=0;ii<t;ii++){
        long long int n,q;
        cin>>n>>q;
        vector<pair<long long int,long long int>>v(n);
        for(int i=0;i<n;i++){
            cin>>v[i].first>>v[i].second;
        }
        vector<pair<long long int,long long int>>vp;
        vp=v;
        Vertex *seg;
        long long int lim=1e18;
        int r=n-1;
        for(int i=0;i<n;i++){
            if(vp[i].first==2){
                r=i-1;
                break;
            }
        }
        seg=build(vp,0,r);
        int full=0;
        for(int i=r+1;i<vp.size();i++){
            if(vp[i].first==2){
                long long int st=0;
                Vertex *nseg=seg;
                while(vp[i].second>0LL){
                    if(vp[i].second&(1LL<<st) || nseg->size+seg->size>lim){
                        Vertex * expseg=new Vertex(seg,nseg);
                        vp[i].second-=(1LL<<st);
                        seg=expseg;
                        if(seg->size>lim){
                            full=1;
                            break;
                        }
                    }
                    Vertex * expseg=new Vertex(nseg,nseg);
                    nseg=expseg;
                    st++;
                }
                if(full){
                    break;
                }
            }
            else{
                int l=i;
                int r=i;
                while(r+1<n && vp[r+1].first==1){
                    r++;
                }
                i=r;
                Vertex *nseg=build(vp,l,r);
                Vertex *jseg=new Vertex(seg,nseg);
                seg=jseg;
                if(seg->size>lim){
                    full=1;
                    break;
                }
            }
        }
        for(int i=0;i<q;i++){
            long long int x;
            cin>>x;
            cout<<get_sum(seg,0LL,seg->size-1LL,x-1LL,x-1LL)<<" ";
        }
        cout<<"\n";
    }
    return 0;
}