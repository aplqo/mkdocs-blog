# Minimum-cost flow

[Primal-dual method](https://www.cnblogs.com/tkandi/p/10532774.html)

设点 $i$ 势函数为 $h(i)$，从 $S$ 到 $i$ 最短路为 $d(i)$

若初始边权非负，则 $h(i)=0$，否则 $h(i)=d_i$

每次增广后令 $h'(i)=h(i)+d(i)$

``` cpp
constexpr int inf = INT_MAX;

struct Edge
{
    unsigned int from, to, id;
    int cap, cost;
    Edge* pre;
} ed[maxm * 2 + 1];
Edge *head[maxn + 1], *cur[maxn + 1];
int dis[maxn + 1], h[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to, const int cap, const int cost)
{
    static Edge* cur = ed;
    cur->from = from;
    cur->to = to;
    cur->id = cur - ed;
    cur->cap = cap;
    cur->cost = cost;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
bool dijkstra(const unsigned int s, const unsigned int t, const unsigned int n)
{
    using pair_t = pair<int, unsigned int>;
    static bool vis[maxn + 1];
    priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
    fill(dis, dis + 1 + n, inf);
    fill(vis, vis + 1 + n, false);
    dis[s] = 0;
    q.emplace(0, s);
    while (!q.empty())
    {
        const auto cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (Edge* i = head[cur.second]; i; i = i->pre)
        {
            const int w = h[i->from] - h[i->to] + i->cost;
            if (!vis[i->to] && i->cap && dis[i->to] > cur.first + w)
            {
                dis[i->to] = cur.first + w;
                q.emplace(dis[i->to], i->to);
            }
        }
    }
    return vis[t];
}
int dfs(const int x, const int cap, const int sink)
{
    static bool vis[maxn + 1];
    if (x == sink || !cap)
        return cap;
    vis[x] = true;
    int rst = cap;
    for (Edge*& i = cur[x]; i && rst; i = i->pre)
    {
        const int w = h[i->from] + i->cost - h[i->to];
        if (!vis[i->to] && i->cap && dis[i->to] == dis[x] + w)
        {
            const int t = dfs(i->to, min(rst, i->cap), sink);
            rst -= t;
            i->cap -= t;
            ed[i->id ^ 1].cap += t;
        }
    }
    vis[x] = false;
    return cap - rst;
}
pair<int, int> flow(const unsigned int n, const unsigned int source, const unsigned int sink)
{
    int mxf = 0, mnc = 0;
    while (dijkstra(source, sink, n))
    {
        copy(head, head + n + 1, cur);
        const int t = dfs(source, inf, sink);
        for (unsigned int i = 1; i <= n; ++i)
            if (dis[i] != inf)
                h[i] += dis[i];
        mxf += t;
        mnc += h[sink] * t;
    }
    return make_pair(mxf, mnc);
}
```
