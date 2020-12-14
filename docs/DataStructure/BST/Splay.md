# Splay

一个节点后都要将其旋转到根节点.

``` cpp
class Splay
{
public:
    int kth(unsigned int k);
    void insert(const int r);
    void remove(const int l, const int r);
    unsigned int size() const;
    void build();

private:
    struct node
    {
        inline void rotate();
        void splay(node* const dest);
        inline void pushup();

        node *chl[2] = {}, *fa = nullptr;
        int val;
        unsigned int cnt = 0, siz = 0;
    } root, SNodes[maxn + 100], *cur = SNodes, *lv, *mv;
    node* pre(const int x);
    node* nxt(const int x);
    node* extract(const int l, const int r);

} spl;
void Splay::insert(const int v)
{
    Splay::node* c = root.chl[1];
    ++root.siz;
    while (true)
    {
        ++(c->siz);
        if (c->val == v)
        {
            ++(c->cnt);
            break;
        }
        const bool typ = c->val < v;
        if (!c->chl[typ])
        {
            cur->val = v;
            cur->fa = c;
            c->chl[typ] = cur++;
        }
        c = c->chl[typ];
    }
    c->splay(&root);
}
void Splay::remove(const int l, const int r)
{
    auto i = extract(l, r);
    if (!i)
        return;
    Splay::node* f = i->fa;
    f->chl[f->chl[1] == i] = nullptr;
    while (f)
    {
        f->pushup();
        f = f->fa;
    }
}
int Splay::kth(unsigned int k)
{
    Splay::node* c = root.chl[1];
    ++k;
    while (true)
    {
        const unsigned int ls = c->chl[0] ? c->chl[0]->siz : 0;
        if (k > ls && k <= c->cnt + ls)
        {
            c->splay(&root);
            return c->val;
        }
        if (k <= ls)
            c = c->chl[0];
        else if (k > c->cnt + ls)
        {
            k -= c->cnt + ls;
            c = c->chl[1];
        }
    }
}
unsigned int Splay::size() const
{
    return root.siz - 3;
}
void Splay::build()
{
    cur->val = INT_MAX;
    cur->siz = 2;
    cur->cnt = 1;
    cur->fa = &root;
    mv = cur;
    root.chl[1] = cur++;

    cur->val = INT_MIN;
    cur->siz = cur->cnt = 1;
    cur->fa = root.chl[1];
    lv = cur;
    root.chl[1]->chl[0] = cur++;

    root.siz = 3;
    root.cnt = 1;
}
void Splay::node::rotate()
{
    Splay::node *const f = fa, *const ff = fa->fa;
    if (ff)
        ff->chl[ff->chl[1] == f] = this;
    const bool typ = f->chl[1] == this;
    f->fa = this;
    fa = ff;
    if (chl[!typ])
        chl[!typ]->fa = f;
    f->chl[typ] = chl[!typ];
    chl[!typ] = f;
    f->pushup();
    pushup();
}
inline void Splay::node::splay(Splay::node* const dest)
{
    while (fa != dest)
    {
        Splay::node* const ff = fa->fa;
        if (ff != dest)
        {
            if ((ff->chl[1] == fa) ^ (fa->chl[1] == this))
                rotate();
            else
                fa->rotate();
        }
        rotate();
    }
}
inline void Splay::node::pushup()
{
    siz = cnt;
    if (chl[0])
        siz += chl[0]->siz;
    if (chl[1])
        siz += chl[1]->siz;
}
Splay::node* Splay::pre(const int x)
{
    Splay::node *c = root.chl[1], *ret = lv;
    int vpr = INT_MIN;
    while (c)
    {
        if (c->val < x && c->val > vpr)
        {
            vpr = c->val;
            ret = c;
        }
        c = c->chl[c->val < x];
    }
    ret->splay(&root);
    return ret;
}
Splay::node* Splay::nxt(const int x)
{
    Splay::node *c = root.chl[1], *ret = mv;
    int vnxt = INT_MAX;
    while (c)
    {
        if (c->val > x && c->val < vnxt)
        {
            vnxt = c->val;
            ret = c;
        }
        c = c->chl[c->val <= x];
    }
    ret->splay(&root);
    return ret;
}
Splay::node* Splay::extract(const int l, const int r)
{
    Splay::node *p = pre(l), *n = nxt(r);
    p->splay(&root);
    n->splay(p);
    return n->chl[0];
}
```
