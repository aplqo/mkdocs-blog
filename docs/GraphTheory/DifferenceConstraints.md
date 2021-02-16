# system of difference constraints

差分约束系统可用于求解 $x_1-x_2 \le c$ 不等式组。

求最大解时是用最短路，最小解使用最长路。

example code

求解

$$
\begin{aligned}
\left\{\begin{align}
    x_{c1}-x_{c2} &\le y_1 \\
    x_{c3}-x_{c4} &\le y_2 \\
    &\vdots \\
    x_{cn-1}-x_{cn} &\le y_n
\end{align} \right.
\end{aligned}
$$

``` cpp
typedef int num;
const int maxn = (5e3 + 10) * 2;
constexpr num ninf = numeric_limits<num>::min();

struct edge {
  int to;
  num val;
  edge* pre = nullptr;
} ed[maxn];
edge* head[maxn];
num ans[maxn];
int dep[maxn];
bool vis[maxn];
int n, m;

inline void AddEdge(int from, int to, num val);
inline bool spfa(int s)
{
  queue<int> q;
  q.push(s);
  dep[s] = 1;
  while (!q.empty()) {
    int cur = q.front();
    q.pop();
    vis[cur] = false;
    for (edge* i = head[cur]; i; i = i->pre)
      if (ans[i->to] < ans[cur] + i->val) {
        ans[i->to] = ans[cur] + i->val;
        if (!vis[i->to]) {
          ++dep[i->to];
          if (dep[i->to] > n) return false;  // no solution
          q.push(i->to);
          vis[i->to] = true;
        }
      }
  }
  return true;  // x_i = ans[i]
}

void build()
{
  cin >> n >> m;
  for (int i = 0; i < m; ++i) {
    int c1, c2;
    num y;
    cin >> c1 >> c2 >> y;
    AddEdge(c1, c2, -y);
  }
  for (int i = 1; i <= n; ++i)
    AddEdge(0, i, (ninf / 2));
  fill(ans + 1, ans + 1 + n, ninf);
  spfa(0);
}
```
