# Inverse series

Let $f_0^{-1}f \equiv 1 \pmod{x^{\lceil \frac{n}{2} \rceil}}$

$$
\begin{align}
f_0^{-1}f &\equiv 1 \pmod{x^{\lceil \frac{n}{2} \rceil}} \\
f^{-1}f &\equiv 1 \pmod{x^{\lceil \frac{n}{2} \rceil}} \\
f_0^{-1}-f^{-1} &\equiv 1 \pmod{x^{\lceil \frac{n}{2} \rceil}} \\
(f_0^{-1}-f^{-1})^2 &\equiv 1 \pmod {x^n} \\
f_0^{-2}-2f_0^{-1}f^{-1}+f^{-2} &\equiv 0 \pmod {x^n}\\
f_0^{-2}f-2f_0^{-1}+f^{-1} & \equiv 0 \pmod{x^n} \\
f^{-1} &\equiv f_0^{-1}(2-f_0^{-1}f) \pmod{x^n}
\end{align}
$$

```cpp
// @tparam typ: 0: dft, 1: idft(include divide every element by n)
// @tparam invalid only copy elements in [0,2^{ign - invalid}) 
template <unsigned int typ, unsigned int invalid>
void dft(const Number src[], Number dest[], const unsigned int lgn);

void inverse(const Number a[], Number out[], const unsigned int lgn)
{
    static Number tmpa[maxn + 10], tmpb[maxn + 10];
    out[0] = quickPow(a[0], mod - 2);
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        initRev(i + 1);
        dft<0, 1>(a, tmpa, i + 1);
        dft<0, 2>(out, tmpb, i + 1);
        {
            const unsigned int sz = 1u << (i + 1);
            for (unsigned int j = 0; j < sz; ++j)
                tmpb[j] = tmpb[j] * (2 - tmpa[j] * tmpb[j]);
        }
        dft<1, 0>(tmpb, out, i + 1);
    }
}
```

pointer version:
```cpp
inline void updateInverse(const ConstArrayPtr &a, const Number old[], Number dest[], const unsigned int n)
{
    convolution(
        a,
        ConstArrayPtr{old, n >> 1},
        ArrayPtr{dest, n << 1},
        [](Number a, Number b) { return b * (2 - a * b); });
}
void inverse(const ConstArrayPtr a, const ArrayPtr dest)
{
    Number *const pDest = dest.ptr;
    pDest[0] = inverse(a.ptr[0]);
    if (dest.size == 1)
        return;
    const unsigned int n = 1u << ceilLg[dest.size], lim = std::max(2u, 1u << ceilLg[a.size]);
    for (unsigned int i = 2; i < lim; i <<= 1)
        updateInverse(ConstArrayPtr{a.ptr, i}, pDest, pDest, i);
    for (unsigned int i = lim; i <= n; i <<= 1)
        updateInverse(a, pDest, pDest, i);
}
```