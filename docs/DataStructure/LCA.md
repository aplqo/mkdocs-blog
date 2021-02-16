# LCA

example code

``` cpp
const int maxn = 500005, maxl = 25;

struct edge {
  int to;
  edge* pre = nullptr;
} ed[maxn * 2];
edge* head[maxn];
int dep[maxn], fa[maxn][maxl], lg[maxn];
int n, m;

void init(int x, int f)
{
  dep[x] = dep[f] + 1;
  fa[x][0] = f;
  for (int i = 1; i <= lg[dep[x]]; ++i)
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
  for (edge* i = head[x]; i != nullptr; i = i->pre) {
    if (dep[i->to]) continue;
    init(i->to, x);
  }
}
inline void getLog(int n)
{
  for (int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
inline int lca(int a, int b)
{
  if (dep[a] != dep[b]) {
    if (dep[a] > dep[b]) swap(a, b);
    int d = dep[b] - dep[a];
    int t = 1;
    for (int i = 0; d; ++i, t <<= 1)
      if (d & t) {
        b = fa[b][i];
        d ^= t;
      }
  }
  if (a == b) return a;
  for (int k = lg[dep[a]] + 1; k;) {
    int m = k - 1;
    if (fa[a][m] == fa[b][m])
      --k;
    else {
      a = fa[a][m];
      b = fa[b][m];
    }
  }
  return fa[a][0];
}
```

## extend

在树上维护所有满足区间可合并的信息 。

e.g. 区间最大值,区间最小值,区间和,区间 gcd,最大子段和
