# Walsh Transform

General code (bit reverse see code of [FFT](./fft.md))

Calcuate $c_i=\sum_{i=j\bigoplus k}a_{j}b_{k}$

``` cpp
unsigned int rev[1u << maxn];

static void initRev(const unsigned int n)  // same as fft
    inline void reverseCopy(unsigned long long dest[],
                            const unsigned long long src[],
                            const unsigned int sz)  // same as fft
    template <class Func>
    void fwt(const unsigned long long src[], unsigned long long dest[],
             const unsigned int n, Func pred)
{
  reverseCopy(dest, src, n);
  for (unsigned int step = 2; step <= n; step <<= 1) {
    const unsigned int half = step >> 1;
    for (unsigned int blk = 0; blk < n; blk += step)
      for (unsigned int i = 0; i < half; ++i)
        pred(dest[blk + i], dest[blk + i + half]);
  }
}
template <class Func, class iFunc>
void fwt(const unsigned int n, const unsigned long long a[],
         const unsigned long long b[], unsigned long long res[], Func pred,
         iFunc ipred)
{
  static unsigned long long tmpa[1u << maxn], tmpb[1u << maxn];
  const unsigned int sz = 1u << n;
  fwt(a, tmpa, sz, pred);
  fwt(b, tmpb, sz, pred);
  for (unsigned int i = 0; i < sz; ++i)
    tmpb[i] = (tmpb[i] * tmpa[i]) % mod;
  fwt(tmpb, res, sz, ipred);
}
```
