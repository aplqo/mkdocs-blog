# Segment tree

``` cpp
const int maxn = 100005;

class node
{
public:
    void build(int, int, long long[maxn], node*(&));
    void modify(int, int, long long);
    long long query(int, int);

private:
    inline void push(long long);

    node *lc = nullptr, *rc = nullptr;
    int lv, rv, mv;
    long long val, laz = 0;
} tr[maxn * 4];
void node::build(int l, int r, long long dat[maxn], node*(&nxt))
{
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    if (r - l == 1)
    {
        val = dat[l];
        return;
    }
    lc = ++nxt;
    lc->build(l, mv, dat, nxt);
    rc = ++nxt;
    rc->build(mv, r, dat, nxt);
    val = lc->val + rc->val;
}
void node::modify(int l, int r, long long val)
{
    if (rv - lv == 1)
    {
        this->val += val;
        return;
    }
    if (l == lv && r == rv)
    {
        this->push(val);
        return;
    }
    if (laz)
    {
        lc->push(laz);
        rc->push(laz);
        laz = 0;
    }
    if (r <= mv)
        lc->modify(l, r, val);
    else if (l >= mv)
        rc->modify(l, r, val);
    else
    {
        lc->modify(l, mv, val);
        rc->modify(mv, r, val);
    }
    this->val = lc->val + rc->val;
}
long long node::query(int l, int r)
{
    if (rv - lv == 1 || (l == lv && r == rv))
        return val;
    if (laz)
    {
        lc->push(laz);
        rc->push(laz);
        laz = 0;
    }
    if (r <= mv)
        return lc->query(l, r);
    else if (l >= mv)
        return rc->query(l, r);
    else
        return lc->query(l, mv) + rc->query(mv, r);
}
inline void node::push(long long laz)
{
    this->laz += laz;
    val += laz * (rv - lv);
}

node *tree = tr;
```

## merge

``` cpp
class STree
{
public:
    void merge(STree* const other)
    {
        if (!other)
            return;
        if (rv - lv == 1)
        {
            this->val += other->val;
            return;
        }
        lc = may(lc, other->lc);
        rc = may(rc, other->rc);
        pushup();
    }

    static STree* cur;

private:
    inline void pushup()
    {
        this->val = get(lc) + get(rc);
    }
    static unsigned int get(STree* c)
    {
        return c ? c->val : 0;
    }
    static STree* may(STree* a, STree*(&other))
    {
        if (!a)
        {
            a = other;
            other = nullptr;
            return a;
        }
        a->merge(other);
        return a;
    }

    unsigned int lv, mv, rv;
    STree *lc, *rc;
    unsigned int val;
};
```
