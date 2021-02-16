# Cartesian tree

笛卡尔树是一种二叉树，每一个结点由一个键值二元组 $(k,w)$ 构成。要求 $k$
满足二叉搜索树的性质，而 $w$ 满足堆的性质。

### build

以序列中元素编号作为 $k$ 建立。

``` cpp
struct node {
  unsigned int val;
  unsigned int lc = 0, rc = 0;
} nodes[maxn + 1];

void buildTree(const unsigned int n)
{
  static unsigned int stk[maxn + 1], *top = stk;
  *(top++) = 1;
  for (unsigned int i = 2; i <= n; ++i) {
    unsigned int* ptr = top;
    while (ptr > stk && nodes[*(ptr - 1)].val > nodes[i].val)
      --ptr;
    if (ptr > stk) nodes[*(ptr - 1)].rc = i;
    if (ptr < top) nodes[i].lc = *ptr;
    top = ptr;
    *(top++) = i;
  }
}
```
