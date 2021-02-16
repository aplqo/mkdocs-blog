# evalution

## One point
```cpp
Number eval(const ConstArrayPtr pol, const Number x)
{
  Number ret = pol.ptr[pol.size - 1];
  if (pol.size > 1) {
    for (unsigned int i = pol.size - 1; i; --i)
      ret = ret * x + pol.ptr[i - 1];
  }
  return ret;
}
```

## multipoint
```cpp
namespace Evaluation {
Number mem[maxn * maxln * 4 * 2 + 10], *cur = mem;
ConstArrayPtr t[maxn * 4 + 10], prod[maxn * 4 + 10];

ConstArrayPtr transMultiply(const Number vx[], const ConstArrayPtr &b,
                            const unsigned int lgn)
{
  static Number tmpb[maxn + 10];
  dft<false>(b, tmpb, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] *= vx[i];
  dft<false>(ConstArrayPtr{tmpb, n}, cur, lgn);
  return ConstArrayPtr{cur, n};
}
void conquer(Number *const dest, const unsigned int lgn)
{
  static unsigned int size[maxn + 10];
  const unsigned int n = 1u << lgn;
  size[1] = n;
  for (unsigned int cid = 1; cid < n; ++cid) {
    static Number tmpv[maxn + 10];
    const unsigned int left = cid << 1, right = left | 1;
    const unsigned int actualSize = size[cid] >> 1;
    const unsigned int childSize = t[cid].size +
                                   std::max(prod[left].size, prod[right].size) -
                                   1,
                       lgn = ceilLg[childSize];
    dft<true>(t[cid], tmpv, lgn);
    t[left] = transMultiply(tmpv, prod[right], lgn);
    t[left].size = size[left] = actualSize;
    cur += actualSize;
    t[right] = transMultiply(tmpv, prod[left], lgn);
    t[right].size = size[right] = actualSize;
    cur += actualSize;
  }
  const ConstArrayPtr *p = t + n;
  for (unsigned int i = 0; i < n; ++i, ++p)
    dest[i] = p->ptr[0];
}

void evaluation(const ConstArrayPtr &a, const ConstArrayPtr &pos, Number dest[])
{
  if (pos.size == 1) {
    dest[0] = eval(a, pos.ptr[0]);
    return;
  }
  else if (a.size == 1) {
    std::fill(dest, dest + pos.size, a.ptr[0]);
    return;
  }
  cur = mem;
  const unsigned int len = std::max(a.size, pos.size), lgn = ceilLg[len],
                     n = 1u << lgn;
  Product::product<false>(pos, lgn, prod, cur, [](Number *cur, Number v) {
    cur[0] = 1;
    cur[1] = -v;
  });
  {
    static Number tmp[maxn + 10];
    ArrayPtr iv{tmp, std::max(n, prod[1].size)}, pv{cur, 0};
    inverse(prod[1], iv);
    ntt<false, true, false>(iv, a, pv);
    pv.size = n;
    t[1] = pv;
    cur += n;
  }
  conquer(dest, lgn);
}
}  // namespace Evaluation
```
