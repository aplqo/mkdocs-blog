# Maximum flow

## ISAP

``` cpp
struct Edge {
  unsigned int from, to, id;
  long long cap, flw;
  Edge* pre;
} ed[maxm * 2 + 1];
Edge *head[maxn + 1], *par[maxn + 1], *cur[maxn + 1];
unsigned int dep[maxn + 1], cnt[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to,
             const long long cap)
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

void bfs(const unsigned int t)
{
  static bool vis[maxn + 1];
  queue<unsigned int> q;
  q.push(t);
  dep[t] = 0;
  vis[t] = true;
  while (!q.empty()) {
    const unsigned int cur = q.front();
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
long long augment(const unsigned int s, const unsigned int t)
{
  long long ret = LLONG_MAX;
  unsigned int x = t;
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
long long isap(const unsigned int s, const unsigned int t, const unsigned int n)
{
  bfs(t);
  for (unsigned int i = 1; i <= n; ++i)
    ++cnt[dep[i]];
  copy(head, head + 1 + n, cur);
  long long ret = 0;
  unsigned int x = s;
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
      unsigned int miv = n;
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
