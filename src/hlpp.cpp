#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <list>
#include <queue>
const int maxn = 1200, maxm = 120000;

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

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  unsigned int n, m, s, t;
  std::cin >> n >> m >> s >> t;
  for (int i = m; i; --i) {
    int u, v, c;
    std::cin >> u >> v >> c;
    if (u != v) addEdge(u - 1, v - 1, c);
  }
  initGraph(n);
  std::cout << hlpp(n, s - 1, t - 1) << "\n";
  return 0;
}