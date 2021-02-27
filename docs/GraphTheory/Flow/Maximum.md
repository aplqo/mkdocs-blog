# Maximum flow

## ISAP

``` cpp
struct Edge {
  int from, to, id;
  long long cap, flw;
  Edge* pre;
} ed[maxm * 2 + 1];
Edge *head[maxn + 1], *par[maxn + 1], *cur[maxn + 1];
unsigned int dep[maxn + 1], cnt[maxn + 1];

void addEdge(const int from, const int to, const long long cap)
{
  static Edge* cur = ed;
  cur->from = from;
  cur->to = to;
  cur->cap = cap;
  cur->flw = 0;
  cur->pre = head[from];
  head[from] = cur;
  cur->id = cur - ed;
  ++cur;
}

void bfs(const int t)
{
  static bool vis[maxn + 1];
  queue<int> q;
  q.push(t);
  dep[t] = 0;
  vis[t] = true;
  while (!q.empty()) {
    const int cur = q.front();
    q.pop();
    for (Edge* i = head[cur]; i; i = i->pre) {
      const Edge& r = ed[i->id ^ 1];
      if (!vis[r.from] && r.cap > r.flw) {
        vis[r.from] = true;
        dep[r.from] = dep[cur] + 1;
        q.push(r.from);
      }
    }
  }
}
long long augment(const int s, const int t)
{
  long long ret = LLONG_MAX;
  int x = t;
  while (x != s) {
    ret = min(ret, par[x]->cap - par[x]->flw);
    x = par[x]->from;
  }
  x = t;
  while (x != s) {
    par[x]->flw += ret;
    ed[par[x]->id ^ 1].flw -= ret;
    x = par[x]->from;
  }
  return ret;
}
long long isap(const int s, const int t, const int n)
{
  bfs(t);
  for (int i = 1; i <= n; ++i)
    ++cnt[dep[i]];
  copy(head, head + 1 + n, cur);
  long long ret = 0;
  int x = s;
  while (dep[s] < n) {
    if (x == t) {
      ret += augment(s, t);
      x = s;
    }
    bool adv = false;
    for (Edge* i = cur[x]; i; i = i->pre)
      if (i->cap > i->flw && dep[i->to] + 1 == dep[x]) {
        adv = true;
        cur[x] = i;
        par[i->to] = i;
        x = i->to;
        break;
      }
    if (!adv) {
      int miv = n;
      for (Edge* i = head[x]; i; i = i->pre)
        if (i->cap > i->flw) miv = min(miv, dep[i->to] + 1);
      --cnt[dep[x]];
      if (!cnt[dep[x]]) break;
      dep[x] = miv;
      ++cnt[dep[x]];
      cur[x] = head[x];
      if (x != s) x = par[x]->from;
    }
  }
  return ret;
}
```

## HLPP

```cpp
typedef int Flow;
struct Edge {
  int from, to;
  mutable Flow flow;
  union {
    int id;
    const Edge* rev;
  };
};
struct Vertex {
  std::vector<Edge> edge;
  const Edge *cur, *begin, *end;
  int h;
  Flow exceed;
  bool inHeap;

  void init();
  void relabel();
  void push(const int source, const int sink);

 private:
  void pushRange(const int source, const int sink, const Edge* beg,
                 const Edge* end);
} vertex[maxn + 10];
struct VertexLess {
  inline bool operator()(const int a, const int b) const
  {
    return vertex[a].h < vertex[b].h;
  }
};
std::priority_queue<int, std::vector<int>, VertexLess> heap;

inline void addEdge(const int from, const int to, const Flow flow)
{
  vertex[from].edge.push_back(Edge{from, to, flow, {vertex[to].edge.size()}});
  vertex[to].edge.push_back(Edge{to, from, 0, {vertex[from].edge.size() - 1}});
}
bool bfs(const int s, const int t)
{
  static bool vis[maxn + 10];
  std::queue<int> q;
  vis[t] = true;
  q.push(t);
  while (!q.empty()) {
    const int cur = q.front();
    q.pop();
    for (const Edge& i : vertex[cur].edge)
      if (i.rev->flow && !vis[i.to]) {
        vertex[i.to].h = vertex[cur].h + 1;
        vis[i.to] = true;
        q.push(i.to);
      }
  }
  return vis[s];
}
void Vertex::relabel()
{
  int mn = INT_MAX;
  for (const Edge& i : edge)
    if (i.flow) mn = std::min(mn, vertex[i.to].h);
  if (mn != INT_MAX) h = mn + 1;
}
template <bool ignCheck>
inline void pushEdge(const Edge* e, const Flow delt, const int source,
                     const int sink)
{
  const int to = e->to;
  e->flow -= delt;
  e->rev->flow += delt;
  vertex[e->from].exceed -= delt;
  vertex[to].exceed += delt;
  if (to != source && to != sink && (ignCheck || !vertex[to].inHeap)) {
    heap.push(to);
    vertex[to].inHeap = true;
  }
}
inline void Vertex::pushRange(const int source, const int sink, const Edge* beg,
                              const Edge* end)
{
  for (const Edge* i = beg; i < end && exceed; ++i)
    if (i->flow && vertex[i->to].h + 1 == h) {
      if (i->flow < exceed)
        pushEdge<false>(i, i->flow, source, sink);
      else {
        cur = i;
        pushEdge<false>(i, exceed, source, sink);
        return;
      }
    }
}
void Vertex::push(const int source, const int sink)
{
  pushRange(source, sink, cur, end);
  if (exceed) pushRange(source, sink, begin, cur);
}
void Vertex::init()
{
  if (!edge.empty()) {
    cur = begin = &edge.front();
    end = &edge.back() + 1;
  }
  else
    cur = begin = end = nullptr;
}

Flow hlpp(const int n, const int source, const int sink)
{
  static int cnt[maxn + 10];
  if (!bfs(source, sink)) return 0;
  std::list<int> lst;
  vertex[source].h = n;
  for (int i = 0; i < n; ++i) {
    ++cnt[vertex[i].h];
    vertex[i].init();
    if (i != source && i != sink) lst.push_back(i);
  }
  for (const Edge& i : vertex[source].edge)
    if (i.flow) pushEdge<true>(&i, i.flow, source, sink);
  while (!heap.empty()) {
    const int cur = heap.top();
    heap.pop();
    vertex[cur].inHeap = false;
    vertex[cur].push(source, sink);
    while (vertex[cur].exceed) {
      --cnt[vertex[cur].h];
      if (!cnt[vertex[cur].h]) {
        for (auto it = lst.cbegin(); it != lst.cend();)
          if (vertex[*it].h > vertex[cur].h) {
            vertex[*it].h = n + 1;
            it = lst.erase(it);
          }
          else
            ++it;
      }
      vertex[cur].relabel();
      ++cnt[vertex[cur].h];
      vertex[cur].push(source, sink);
    }
  }
  return vertex[sink].exceed;
}
void initGraph(const int n)
{
  for (unsigned int i = 0; i < n; ++i)
    for (Edge& j : vertex[i].edge)
      j.rev = &vertex[j.to].edge[j.id];
}
```
