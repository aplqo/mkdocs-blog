# Link cut tree

## Normal version (Not support lca)

``` cpp
class LinkCutTree {
 public:
  void build(const unsigned int n, const unsigned int val[]);
  unsigned int xorSum(const unsigned int x, const unsigned int y);
  void modify(const unsigned int pos, const unsigned int val);
  void link(const unsigned int x, const unsigned int y);
  void cut(const unsigned int x, const unsigned int y);

 private:
  struct SplayNode {
    inline void pushdown();
    inline void pushup();
    inline void pushRev();
    inline bool isRoot();
    void pushPath();
    void splay();
    void rotate();

    bool rev = false;
    unsigned int val = 0, xsum = 0;
    SplayNode *fa = nullptr, *chl[2] = {};
  };
  SplayNode snodes[maxn + 10];

  void access(SplayNode* x);
  void makeRoot(SplayNode* x);
  SplayNode* findRoot(SplayNode* x);
  void split(SplayNode* x, SplayNode* y);
  static inline SplayNode* maybe(SplayNode* const x);
} lct;
unsigned int val[maxn + 10];

void LinkCutTree::build(const unsigned int n, const unsigned int val[])
{
  for (unsigned int i = 1; i <= n; ++i)
    snodes[i].val = snodes[i].xsum = val[i];
}
unsigned int LinkCutTree::xorSum(const unsigned int x, const unsigned int y)
{
  if (x == y) return snodes[x].val;
  SplayNode *const px = snodes + x, *const py = snodes + y;
  if (findRoot(px) != findRoot(py)) return 0;
  split(px, py);
  return py->xsum;
}
void LinkCutTree::modify(const unsigned int x, const unsigned int val)
{
  SplayNode* const px = snodes + x;
  px->splay();
  px->val = val;
  px->pushup();
}
void LinkCutTree::link(const unsigned int x, const unsigned int y)
{
  SplayNode* const px = snodes + x;
  makeRoot(px);
  if (findRoot(snodes + y) != px) px->fa = snodes + y;
}
void LinkCutTree::cut(const unsigned int x, const unsigned int y)
{
  SplayNode *const px = snodes + x, *const py = snodes + y;
  split(px, py);
  if (py->chl[0] != px || px->chl[1] || px->fa != py) return;
  py->chl[0] = px->fa = nullptr;
  py->pushup();
}
void LinkCutTree::access(LinkCutTree::SplayNode* x)
{
  for (SplayNode* lst = nullptr; x; x = x->fa) {
    x->splay();
    x->chl[1] = lst;
    x->pushup();
    lst = x;
  }
}
void LinkCutTree::makeRoot(LinkCutTree::SplayNode* x)
{
  access(x);
  x->splay();
  x->pushRev();
}
LinkCutTree::SplayNode* LinkCutTree::findRoot(LinkCutTree::SplayNode* x)
{
  access(x);
  x->splay();
  while (x->chl[0])
    x = x->chl[0];
  x->splay();
  return x;
}
void LinkCutTree::split(LinkCutTree::SplayNode* x, LinkCutTree::SplayNode* rt)
{
  makeRoot(x);
  access(rt);
  rt->splay();
}
void LinkCutTree::SplayNode::pushRev()
{
  swap(chl[0], chl[1]);
  rev ^= 1;
}
inline void LinkCutTree::SplayNode::pushdown()
{
  if (!rev) return;
  maybe(chl[0])->pushRev();
  maybe(chl[1])->pushRev();
  rev = false;
}
inline void LinkCutTree::SplayNode::pushup()
{
  xsum = maybe(chl[0])->xsum ^ maybe(chl[1])->xsum ^ val;
}
void LinkCutTree::SplayNode::pushPath()
{
  if (!isRoot()) fa->pushPath();
  pushdown();
}
inline bool LinkCutTree::SplayNode::isRoot()
{
  return fa ? fa->chl[0] != this && fa->chl[1] != this : true;
}
void LinkCutTree::SplayNode::rotate()
{
  SplayNode *const f = fa, *const ff = f->fa;
  if (!f->isRoot()) ff->chl[ff->chl[1] == f] = this;
  const bool typ = f->chl[1] == this;
  f->fa = this;
  fa = ff;
  if (chl[!typ]) chl[!typ]->fa = f;
  f->chl[typ] = chl[!typ];
  chl[!typ] = f;
  f->pushup();
  this->pushup();
}
void LinkCutTree::SplayNode::splay()
{
  pushPath();
  while (!isRoot()) {
    SplayNode* const ff = fa->fa;
    if (!fa->isRoot()) {
      if ((ff->chl[1] == fa) ^ (fa->chl[1] == this))
        this->rotate();
      else
        fa->rotate();
    }
    this->rotate();
  }
}
inline LinkCutTree::SplayNode* LinkCutTree::maybe(
    LinkCutTree::SplayNode* const x)
{
  static SplayNode nil;
  return x ? x : &nil;
}
```

## Lca query

``` cpp
struct SplayNode {
  inline bool isRoot() const;  // Same as above
  void rotate();               // Same as above
  void splay();                // Same as above

  SplayNode *fa = nullptr, *chl[2] = {};
};
SplayNode vertex[maxn + 10];

void access(SplayNode* x)
{
  for (SplayNode* lst = nullptr; x; x = x->fa) {
    x->splay();
    x->chl[1] = lst;
    lst = x;
  }
}

void link(SplayNode* const rt, SplayNode* const father)
{
  access(rt);
  rt->fa = father;
}
void cut(SplayNode* const x)
{
  access(x);
  x->splay();
  x->chl[0]->fa = nullptr;
  x->chl[0] = nullptr;
}
SplayNode* lca(SplayNode* const x, SplayNode* const y)
{
  access(x);
  access(y);
  x->splay();
  return x->fa ? x->fa : x;
}
```
