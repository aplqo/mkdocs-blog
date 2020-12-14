# k-Shortest

## A\*

估价函数：$f(x)=g(x)+h(x)$ ，$g(x)$ 为从起点到当前点已经过距离，$h(x)$
为当前点到终点最短路长度。

使用优先队列维护 $f(x)$ 最小值。

因为在终点 $h(x)=0$ ，在终点第 $k$ 出队的 $f(x)$ 为第 $k$ 短路。

``` cpp
using num_t = unsigned int;
template <class T>
using heap_t = priority_queue<T, vector<T>, greater<T>>;

class graph
{
public:
    struct edge
    {
        unsigned int to;
        num_t val;
        edge* pre = nullptr;
    };
    const edge* GetEdge(const unsigned int x) const;
    inline void AddEdge(const unsigned int from, const unsigned int to, const num_t val);
} g, r; // original graph, reversed graph

num_t dis[maxn + 10]; // shortest path from i to end

inline void AStar(const unsigned int n, unsigned int k)
{
    using tuple_t = tuple<num_t, num_t, unsigned int>;
    heap_t<tuple_t> q;
    q.push(make_tuple(dis[n], 0, n));
    while (!q.empty())
    {
        tuple_t cur = q.top();
        q.pop();
        if (get<2>(cur) == 1)
        {
            cout << get<0>(cur) << endl;
            --k;
            if (!k)
                break;
        }
        for (const graph::edge* i = g.GetEdge(get<2>(cur)); i; i = i->pre)
        {
            const num_t f = get<1>(cur) + i->val;
            q.push(make_tuple(f + dis[i->to], f, i->to));
        }
    }
    for (; k; --k)
        cout << "-1" << endl;
}
```
