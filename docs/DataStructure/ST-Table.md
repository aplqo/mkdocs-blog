# st table

ST 算法只适用于静态区间求最值

example code

``` cpp
typedef int_fast32_t num;
constexpr int maxn = 1e5 + 5, maxl = 25;

int n;
num dat[maxn], table[maxn][maxl];
num lg[maxn];

num getLog(int x)
{
  if (lg[x]) return lg[x];
  return x > 1 ? lg[x] = getLog(x >> 1) + 1 : 0;
}
inline void init(num dat[maxn], int len)
{
  for (int i = 0; i < len; ++i)
    table[i][0] = dat[i];
  for (num j = 1; j <= getLog(n); ++j)
    for (int i = 0; i + (1 << j) <= n; ++i)
      table[i][j] = max(
          table[i][j - 1],
          table[i + (1 << j - 1)][j - 1]);  // i + 1 <<j - 1 = (i + 1)<<(j -1 )
}
num query(int l, int r)
{
  num d = getLog(r - l + 1);
  return max(table[l][d], table[r - (1 << d) + 1][d]);
}
```

## extend

可处理一切满足如下性质的数据：支持有重叠区间信息快速合并

如： min/max, gcd
