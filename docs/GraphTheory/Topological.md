# Topological sort

时间复杂度：$\text{O}(n+m)$

``` cpp
const int maxn = 1e5, maxm = 1e5;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxm + 10];
unsigned int degree[maxn + 10], seq[maxn + 10], *en = seq;
edge* head[maxn + 10];

inline void topological(unsigned int n)
{
  queue<unsigned int>, greater<unsigned int> > q;
  for (unsigned int i = 1; i <= n; ++i)
    if (!degree[i]) q.push(i);
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    *(en++) = cur;
    for (const edge* i = head[cur]; i; i = i->pre) {
      --(degree[i->to]);
      if (!degree[i->to]) q.push(i->to);
    }
  }
}
```
