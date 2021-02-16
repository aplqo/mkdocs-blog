# NTT

```cpp
template <bool inv>
void dft(const ConstArrayPtr a, Number dest[], const unsigned int lgn)
{
  reverseCopy(dest, a, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 1; i <= lgn; ++i) {
    const unsigned int step = 1u << i, half = 1u << (i - 1);
    const Number wm = unit[inv][i];
    for (Number *blk = dest; blk < dest + n; blk += step) {
      Number w = 1;
      for (Number *j = blk, *k = blk + half; j < blk + half; ++j, ++k) {
        const Number u = *j, v = *k * w;
        *j = u + v;
        *k = u - v;
        w *= wm;
      }
    }
  }
  if constexpr (inv) {
    const Number iv = quickPow(n, mod - 2);
    for (Number *i = dest; i < dest + n; ++i)
      *i *= iv;
  }
}

template <bool op1 = false, bool op2 = false, bool op3 = true, class Func>
void convolution(const ConstArrayPtr &a, const ConstArrayPtr &b,
                 const ArrayPtr dest, const Func pred)
{
  static Number tmpa[maxn + 10], tmpb[maxn + 10];
  const unsigned int lgn = ceilLg[dest.size], n = 1u << lgn;
  dft<op1>(a, tmpa, lgn);
  dft<op2>(b, tmpb, lgn);
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] = pred(tmpa[i], tmpb[i]);
  dft<op3>(ConstArrayPtr{tmpb, n}, dest.ptr, lgn);
}
template <bool op1 = false, bool op2 = false, bool op3 = true>
inline void ntt(const ConstArrayPtr a, const ConstArrayPtr b, ArrayPtr &dest)
{
  dest.size = a.size + b.size - 1;
  convolution<op1, op2, op3>(a, b, dest, std::multiplies<Number>());
}
```
