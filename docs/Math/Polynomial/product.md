# Product

```cpp
namespace Product {
template <bool inPlace>
inline void multiply(ConstArrayPtr &now, const ConstArrayPtr *left,
                     Number *&cur)
{
  ArrayPtr tp;
  if (inPlace)
    tp.ptr = const_cast<Number *>(now.ptr);
  else
    tp.ptr = cur;
  ntt(*left, *(left + 1), tp);
  if (!inPlace) cur += tp.size;
  now = tp;
}
template <bool inPlace, class Func>
void product(const ConstArrayPtr a, const unsigned int lgn,
             ConstArrayPtr *const prod, Number *&cur, Func ini)
{
  const unsigned int msk = 1u << lgn;
  {
    static const Number zero;
    for (unsigned int i = 0; i < a.size; ++i) {
      ini(cur, a.ptr[i]);
      prod[i | msk] = ConstArrayPtr{cur, 2};
      cur += 2;
    }
    if (a.size < msk) {
      *cur = 1;
      const unsigned int cid = a.size | msk;
      prod[cid] = ConstArrayPtr{cur, 1};
      ++cur;
      if (cid + 1 < (msk << 1))
        fill(prod + cid + 1, prod + (msk << 1), prod[cid]);
    }
  }
  for (unsigned int cid = msk - 1; cid > 1; --cid) {
    const unsigned int left = cid << 1, right = left | 1;
    if (prod[right].size == 1 && prod[right].ptr[0] == 1)
      prod[cid] = prod[left];
    else
      multiply<false>(prod[cid], prod + left, cur);
  }
  multiply<inPlace>(prod[1], prod + 2, cur);
}

template <class Func>
inline void product(const ConstArrayPtr &a, ArrayPtr &dest, Func ini)
{
  if (a.size == 1) {
    ini(dest.ptr, a.ptr[0]);
    dest.size = 2;
    return;
  }
  static Number mem[maxn * maxln * 4 + 10];
  static ConstArrayPtr prod[maxn * 4 + 10];
  prod[1] = ConstArrayPtr{dest.ptr, 0};
  Number *cur = mem;
  product<true>(a, ceilLg[a.size], prod, cur, ini);
  dest.size = prod[1].size;
}
}  // namespace Product
```
