# shortest path

## dijkstra

时间复杂度：$\text{O}((n+m)\log m)$

不能用于有负权的图。

``` cpp
typedef pair<int, int> pairType;
typedef priority_queue<pairType, vector<pairType>, greater<pairType>> que;
typedef int num;
const int maxn = 1e5 + 5, maxm = 2e5 + 5;

struct edge
{
    int to;
    num val;
    edge* pre = nullptr;
} ed[maxm];
edge* head[maxn];
num ans[maxn];
bool vis[maxn];

void dijkstra(int s)
{
    que q;
    q.push(make_pair(0, s));
    fill(ans, ans + n + 1, numeric_limits<num>::max());
    ans[s] = 0;
    while (!q.empty())
    {
        pairType cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (edge* i = head[cur.second]; i; i = i->pre)
        {
            if (ans[i->to] > cur.first + i->val)
            {
                ans[i->to] = cur.first + i->val;
                q.push(make_pair(ans[i->to], i->to));
            }
        }
    }
}
```

## spfa

时间复杂度：$\text{O}(km)$ （最坏 $\text{O}(nm)$ ）

当某一点入队大于等于 $n$ 次时图中存在负环。

``` cpp
typedef int numType;
const numType inf = (numType(1) << 31) - 1;
const int maxn = 1e4 + 5, maxm = 5e5 + 5;

struct edge
{
    int to;
    numType val;
    edge* pre = nullptr;
} ed[maxm];
edge* head[maxn];
numType ans[maxn];
bool vis[maxn];
int n;

void spfa(int s)
{
    queue<int> q;
    q.push(s);
    fill(ans, ans + n + 1, inf);
    vis[s] = true;
    ans[s] = 0;
    while (!q.empty())
    {
        int cur = q.front();
        q.pop();
        vis[cur] = false;
        for (edge* i = head[cur]; i != nullptr; i = i->pre)
        {
            if (ans[i->to] > ans[cur] + i->val)
            {
                ans[i->to] = ans[cur] + i->val;
                if (!vis[i->to])
                {
                    q.push(i->to);
                    vis[i->to] = true;
                }
            }
        }
    }
}
```
