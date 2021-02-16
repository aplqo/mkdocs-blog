# Strongly connected compents

## tarjan

时间复杂度：$\text{O}(n+m)$

求得的强连通分量是逆拓扑序。

``` cpp
constexpr int maxn = 100, maxm = maxn * maxn;

template <class T>
struct edge {
  T *from, *to;
  edge* pre = nullptr;
};
struct scc {
  edge<scc>* head = nullptr;
  unsigned int degree[2] = {};
  bool vis = false;

  static edge<scc> ed[maxm + 10], *cur;
  static inline void CreateGraph();
} sc[maxn + 10], *en = sc;
edge<scc> scc::ed[maxm + 10];
edge<scc>* scc::cur = scc::ed;
struct vertex {
  edge<vertex>* head = nullptr;
  scc* s = nullptr;
  unsigned int dfn = 0, low = 0;

  static edge<vertex> ed[maxm + 10], *cur;

  void AddEdge(const unsigned int to);  // add edge this->to
} ve[maxn + 10];
edge<vertex> vertex::ed[maxm + 10];
edge<vertex>* vertex::cur = vertex::ed;

void tarjan(vertex* const x)
{
  static unsigned int sta = 0, top = 0;
  static vertex* stk[maxn + 10];
  stk[++top] = x;
  x->dfn = x->low = ++sta;
  for (auto* i = x->head; i; i = i->pre) {
    vertex* const to = i->to;
    if (!to->dfn) {
      tarjan(to);
      x->low = min(x->low, to->low);
    }
    else if (!to->s)
      x->low = min(x->low, to->dfn);
  }
  if (x->dfn == x->low) {
    x->s = en;
    while (stk[top] != x) {
      stk[top]->s = en;
      --top;
    }
    --top;
    ++en;
  }
}
```

## Construct DAG

排序后去除重边。

空间紧张时可用联合体。

``` cpp
inline void scc::CreateGraph()
{
  for (edge<vertex>* i = vertex::ed; i < vertex::cur; ++i) {
    if (i->from->s != i->to->s) {
      cur->from = i->from->s;
      cur->to = i->to->s;
      ++cur;
    }
  }
  sort(ed, cur, [](const edge<scc>& a, const edge<scc>& b) -> bool {
    return a.from != b.from ? a.from < b.from : a.to < b.to;
  });
  cur = unique(ed, cur, [](const edge<scc>& a, const edge<scc>& b) {
    return a.from != b.from ? false : a.to == b.to;
  });
  for (edge<scc>* i = ed; i < cur; ++i) {
    ++(i->to->degree[0]);
    ++(i->from->degree[1]);
    i->pre = i->from->head;
    i->from->head = i;
  }
}
```
