mt19937 eng{ std::random_device{}() } ;
uniform_int_distribution< ll > uid ( 0 , (int)1e9 ) ;

struct node {
    int val, pr, sz , ans;
    bool lz = 0;
    int lazy = 0 ;
    node *lf = 0, *rt = 0;

    node(int val) : val(val) , ans(val), sz(1) , lazy(0) , pr(eng()) {}
};

class treap {
private:
    node *root = nullptr;

    // test sum ;
    void pull( node *u ){
        u->ans = u->val ;
        if( u->rt ) u->ans += u->rt->ans  ;
        if (u->lf)  u->ans += u->lf->ans  ;
    }

    int cnt(node *u) {
        return u ? u->sz : 0;
    }

    void pushDown(node *u) {
        if (u && u->lz) {
            if (u->rt) u->rt->lz = !u->rt->lz ;
            if (u->lf) u->lf->lz = !u->lf->lz ;
            swap(u->lf, u->rt);
            u->lz = 0;
        }
        if (u) {
            if (u->rt) u->rt->lazy += u->lazy , u->rt->val += u->lazy ;
            if (u->lf) u->lf->lazy += u->lazy , u->lf->val += u->lazy ;
            u->lazy = 0;
        }
    }

    void updateNode(node *u) {
        pushDown(u);
        if (u) {
            u->sz = 1 + cnt(u->lf) + cnt(u->rt);
            pull( u );
        }
    }

    void split(node *u, int preSz, node *&lf, node *&rt) {
        pushDown(u);
        if (!u) return lf = rt = 0, void();
        int szLf = cnt(u->lf);
        if (szLf < preSz)
            split(u->rt, preSz - szLf - 1, u->rt, rt), lf = u;
        else
            split(u->lf, preSz, lf, u->lf), rt = u;
        updateNode(u);
    }

    void merge(node *lf, node *rt, node *&u) {
        pushDown(lf), pushDown(rt);
        if (!lf || !rt) u = lf ? lf : rt;
        else if (lf->pr > rt->pr) merge(lf->rt, rt, lf->rt), u = lf;
        else merge(lf, rt->lf, rt->lf), u = rt;
        updateNode(u);
    }


    void dfs(node *u) {
        pushDown(u);
        if (!u) return;
        dfs(u->lf);
        printf("%d ", u->val);
        dfs(u->rt);
    }

    int &kth(node *u, int k) {
        pushDown(u);
        //if (!u) return -1;
        int szLf = cnt(u->lf);
        if (szLf > k) return kth(u->lf, k);
        else if (szLf == k) return u->val  ;
        return kth(u->rt, k - szLf - 1);
    }

public:
    void insert(int i, int val) {
        node *lf, *rt;
        split(root, i, lf, rt);
        merge(lf, new node(val), lf);
        merge(lf, rt, root);
    }

    void init( vector< int >&a ){
        int n = a.size() ;
        for( int i = 0 ; i<n ; i++ )
            insert( i , a[i] );
    }

    void erase(int i) {
        node *lf, *md, *rt;
        split(root, i, lf, rt);
        split(rt, 1, md, rt);
        delete md;
        merge(lf, rt, root);
    }

    void erase(int i , int size) {
        node *lf, *md, *rt;
        split(root, i, lf, rt);
        split(rt, size, md, rt);
        delete md;
        merge(lf, rt, root);
    }

    void print() {
        dfs(root);
        puts("");
    }

    int &operator[](int i) {
        return kth(root, i);
    }

    void cycleShiftRight(int qs, int qe, int v) {
        node *lf, *md, *rt, *x, *y;
        split(root, qe + 1, lf, rt);
        split(lf, qs, lf, md);

        split(md, md->sz - v, x, y);
        merge(y, x, md);

        merge(lf, md, lf);
        merge(lf, rt, root);
    }

    void reverse(int qs, int qe) {
        node *lf, *md, *rt ;
        split(root, qe + 1, lf, rt);
        split(lf, qs, lf, md);

        if (md) md->lz = !md->lz;

        merge(lf, md, lf);
        merge(lf, rt, root);
    }

    void updateRange( int qs, int qe, int value) {
        node *lf, *md, *rt ;

        split(root, qe + 1 , lf, rt);
        split(lf, qs, lf, md);

        if (md) md->lazy += value , md->val += value ;

        merge(lf, md, lf);
        merge(lf, rt, root);
    }

    int query( int qs , int qe ){
        node *lf, *md, *rt ;

        split(root, qe + 1 , lf, rt);
        split(lf, qs, lf, md);

        int ans  = md->ans ;

        merge(lf, md, lf);
        merge(lf, rt, root);

        return ans;
    }

    int size( ){ return root->sz; }

}tr;
