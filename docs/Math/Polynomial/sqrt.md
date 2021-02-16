# sqrt

```cpp
void sqrt(const ConstArrayPtr a, const ArrayPtr dest)
{
  static Number memInv[maxn + 10], memTinv[maxn + 10], tmpMem[maxn + 10];
  static const auto updateSqrt = [](const ConstArrayPtr a, Number dest[],
                                    const unsigned int n) {
    updateInverse(ConstArrayPtr{dest, n >> 1}, memInv, memTinv, n);
    ArrayPtr tmp{tmpMem};
    ntt(ConstArrayPtr{memTinv, n}, a, tmp);
    {
      const unsigned int half = n >> 1;
      for (unsigned int j = 0; j < half; ++j)
        dest[j] = inv2 * (dest[j] + tmpMem[j]);
      for (unsigned int j = half; j < n; ++j)
        dest[j] = inv2 * tmpMem[j];
    }
    updateInverse(ConstArrayPtr{dest, n}, memInv, memInv, n);
  };
  const unsigned int lgn = ceilLg[dest.size], lim = ceilLg[a.size];
  memInv[0] = dest.ptr[0] = 1;
  if (dest.size == 1) return;
  for (unsigned int i = 1; i < lim; ++i)
    updateSqrt(ConstArrayPtr{a.ptr, 1u << i}, dest.ptr, 1u << i);
  for (unsigned int i = lim; i <= lgn; ++i)
    updateSqrt(a, dest.ptr, 1u << i);
}
```
