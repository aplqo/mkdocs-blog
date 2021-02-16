# ln exp

## ln
```cpp
void ln(const ConstArrayPtr a, ArrayPtr dest)
{
  static Number tpMem[maxn + 10], ivMem[maxn + 10];
  ArrayPtr tp{tpMem}, iv{ivMem, dest.size};
  derivative(a, tp);
  inverse(a, iv);
  ntt(tp, iv, iv);
  iv.size = dest.size;
  integral(iv, dest);
}
```

## exp
```cpp
void exp(const ConstArrayPtr a, const ArrayPtr dest)
{
  static Number tmpMem[maxn + 10];
  dest.ptr[0] = 1;
  const unsigned int lgn = ceilLg[dest.size];
  ArrayPtr pDest{dest.ptr};
  for (unsigned int i = 1; i <= lgn; ++i) {
    const unsigned int n = 1u << i;
    ArrayPtr tmp{tmpMem, n};
    ln(ConstArrayPtr{dest.ptr, 1u << (i - 1)}, tmp);
    {
      const unsigned int lim = std::min(a.size, n);
      for (unsigned int i = 0; i < lim; ++i)
        tmpMem[i] = a.ptr[i] - tmpMem[i];
      for (unsigned int i = lim; i < n; ++i)
        tmpMem[i] = -tmpMem[i];
    }
    tmpMem[0] += 1;
    ntt(tmp, ConstArrayPtr{dest.ptr, n >> 1}, pDest);
  }
}
```
