#include<bits/stdc++.h>
using namespace std;

#define nline "\n"

//segment tree data in the node
struct segdata{
    long long int val=0;
    // long long int mi=LLONG_MAX;
    // long long int ma=LLONG_MIN;
    long long int size=0;
    long long int a=1;
    long long int b=0;
    long long int d=0;
    segdata(){
        
    }
    segdata(long long int data){
        val=data;
        // mi=data;
        // ma=data;
        size=1LL;
    }
    void applydatal(segdata t,segdata rd) {
        val=val*t.a+size*t.b+t.d*((size*(size-1))/2LL);
        // mi=mi*t.a+t.b;
        // ma=ma*t.a+t.b;
        a=a*t.a;
        b+=t.b;
        d+=t.d;
    }
    void applydatar(segdata t,segdata ld) {
        val=val*t.a+size*(t.b+t.d*ld.size)+t.d*((size*(size-1))/2LL);
        // mi=mi*t.a+t.b;
        // ma=ma*t.a+t.b;
        a=a*t.a;
        b+=ld.size*t.d+t.b;
        d+=t.d;
    }
    void resettag(){
        a=1LL;
        b=0LL;
        d=0ll;
    }
    void pulldata(segdata a){
        val+=a.val;
        // mi=min(a.mi,mi);
        // ma=max(a.ma,ma);
        size+=a.size;
    }
    void pulldata(segdata a,segdata b){
        val=a.val+b.val;
        size=a.size+b.size;
        // mi=min(a.mi,b.mi);
        // ma=max(a.ma,b.ma);
    }
};

struct segtree{
    segtree *l,*r;
    segdata data;
    segtree(long long int val): l(nullptr),r(nullptr),data(val){}
    segtree(segtree *l,segtree *r): l(l),r(r),data(segdata()){
        if(l){
            data.pulldata(l->data);
        }
        if(r){
            data.pulldata(r->data);
        }
    }
};


segtree* build(vector<long long int>& a,int tl,int tr){
    if(tl==tr){
        return new segtree(a[tl]);
    }
    int mid=(tl+tr)/2;
    return new segtree(build(a,tl,mid),build(a,mid+1,tr));
}

segdata pullsegdata(segtree* v){
    return (v)?v->data:segdata();
}

segdata get_prod(segtree* v, int tl, int tr, int &l, int &r) {
    // cout<<"qur "<<tl<<" "<<tr<<" "<<v->data.val<<" "<<v->data.b<<" "<<v->data.d<<endl;
    if (tl>r || tr<l)
        return segdata();
    if (tl>=l && tr<=r){
        return v->data;
    }
    int tm = (tl + tr) / 2;
    segdata c;
    if(v->l){
        v->l->data.applydatal(v->data,pullsegdata(v->r));
    }
    if(v->r){
        v->r->data.applydatar(v->data,pullsegdata(v->l));
    }
    v->data.resettag();
    c.pulldata(get_prod(v->l,tl,tm,l,r),get_prod(v->r,tm+1,tr,l,r));
    v->data.pulldata(pullsegdata(v->l),pullsegdata(v->r));
    return c;
}

void rangeop(segtree* v, int tl, int tr, int &l, int &r,segdata &d) {
    // cout<<"upd "<<tl<<" "<<tr<<" "<<v->data.val<<" "<<v->data.b<<" "<<v->data.d<<endl;
    if (tl>r || tr<l)
        return;
    if (tl>=l && tr<=r){
        segdata c;
        c.size=tl-l;
        v->data.applydatar(d,c);
        return;
    }
    int tm = (tl + tr) / 2LL;
    if(v->l){
        v->l->data.applydatal(v->data,pullsegdata(v->r));
    }
    if(v->r){
        v->r->data.applydatar(v->data,pullsegdata(v->l));
    }
    v->data.resettag();
    rangeop(v->l,tl,tm,l,r,d);
    rangeop(v->r,tm+1,tr,l,r,d);
    v->data.pulldata(pullsegdata(v->l),pullsegdata(v->r));
    return;
}

// segment tree prefix descent
// int findfirstprefix(segtree* v, int tl, int tr, int &l, int &r, segdata &d){
//     return 
// }

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n,q;
    cin>>n>>q;
    vector<long long int>v(n);
    for(int i=0;i<n;i++){
        cin>>v[i];
    }
    segtree *seg;
    seg=build(v,0,n-1);
    for(int i=0;i<q;i++){
        int l,r;long long int x,t;
        cin>>t>>l>>r;
        l--;
        r--;
        if(t==1){
            segdata dd;
            dd.d=1LL;
            dd.b=1LL;
            rangeop(seg,0,n-1,l,r,dd);
        }
        else{
            segdata dd=get_prod(seg,0,n-1,l,r);
            cout<<dd.val<<"\n";
        }
    }
    return 0;
}