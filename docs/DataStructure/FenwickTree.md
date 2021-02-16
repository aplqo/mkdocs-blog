# Fenwick Tree

Example code

``` cpp
const int maxn = 5e5 + 5;
typedef unsigned int num;
#define lowbit(x) ((x) & -(x))

num tree[maxn];
int n;

void update(int x, num val)
{
  for (int i = x; i <= n; i += lowbit(i))
    tree[i] += val;
}
num sum(int x)
{
  if (x == 0) return 0;
  num ret = 0;
  for (; x; x -= lowbit(x))
    ret += tree[x];
  return ret;
}
```

### Two-dimensional

``` cpp
unsigned int n, m;  // n*m
class FTree {
 public:
#define lowbit(x) ((x) & -(x))
  void modify(unsigned int x, const unsigned int y, const int v)
  {
    for (; x <= n; x += lowbit(x))
      for (unsigned int j = y; j <= m; j += lowbit(j))
        dat[x][j] += v;
  }
  inline int sum(const unsigned int x1, const unsigned int y1,
                 const unsigned int x2, const unsigned int y2) const
  {
    return query(x2, y2) + query(x1 - 1, y1 - 1) - query(x1 - 1, y2) -
           query(x2, y1 - 1);
  }

 private:
  int query(unsigned int x, const unsigned int y) const
  {
    int ret = 0;
    for (; x; x -= lowbit(x))
      for (unsigned int j = y; j; j -= lowbit(j))
        ret += dat[x][j];
    return ret;
  }
  int dat[maxn + 1][maxn + 1] = {};
};
```
