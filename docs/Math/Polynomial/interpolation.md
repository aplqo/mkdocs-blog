# interpolation

## Lagrange interpolation

Let 

$$
\begin{align}
l(x)&=(x-x_0)(x-x_1)\dots (x-x_n) \\
w_j&=\frac{1}{\prod_{j \neq i}(x_i-x_j)}
\end{align}
$$  

Lagrange's formula: $L(x)=l(x)\sum_{j=0}^{k}\frac{w_j}{x-x_j}y_j$

将以上的拉格朗日插值多项式用来对函数$g(x)=1$插值，可以得到

$$
\begin{align}
g(x)&=l(x)\sum_{j=0}^{k}\frac{w_j}{x-x_j} \\
\frac{L(x)}{g(x)}&=\frac{\sum_{j=0}^{k}\frac{w_j}{x-x_j}y_j}{\sum_{j=0}^{k}\frac{w_j}{x-x_j}} \\
L(x)&=\frac{\sum_{j=0}^{k}\frac{w_j}{x-x_j}y_j}{\sum_{j=0}^{k}\frac{w_j}{x-x_j}}
\end{align}
$$

Example code:
```cpp
Number w[maxn + 10], x[maxn + 10], y[maxn + 10];

void insert(const unsigned int k, const Number cx, const Number cy)
{
    Number acc = 1;
    for (unsigned int i = 0; i < k; ++i)
    {
        w[i] = w[i] / (x[i] - cx);
        acc *= cx - x[i];
    }
    x[k] = cx;
    y[k] = cy;
    w[k] = 1 / acc;
}
```

## optimized
```cpp
namespace Interpolation
{
    Number mem[maxn * maxln * 4 * 2 + 10], *cur = mem;
    ConstArrayPtr f[maxn * 4 + 10], prod[maxn * 4 + 10];

    template <bool inPlace>
    inline void merge(const unsigned int cid)
    {
        static Number tmpa[maxn + 10], tmpb[maxn + 10], tmpc[maxn + 10], tmpd[maxn + 10];
        const unsigned int left = cid << 1, right = left | 1;
        if (f[right].size == 1 && f[right].ptr[0] == 0 && prod[right].size == 1 && prod[right].ptr[0] == 1)
        {
            if constexpr (inPlace)
            {
                std::copy(f[left].ptr, f[left].ptr + f[left].size, const_cast<Number *>(f[cid].ptr));
                f[cid].ptr = f[left].ptr;
            }
            else
                f[cid] = f[left];
            return;
        }
        const unsigned int actualLen = max(f[left].size + prod[right].size, f[right].size + prod[left].size) - 1;
        const unsigned int lgn = ceilLg[actualLen], n = 1u << lgn;
        dft<false>(f[left], tmpa, lgn);
        dft<false>(prod[left], tmpb, lgn);
        dft<false>(f[right], tmpc, lgn);
        dft<false>(prod[right], tmpd, lgn);
        for (unsigned int i = 0; i < n; ++i)
            tmpd[i] = tmpa[i] * tmpd[i] + tmpb[i] * tmpc[i];
        if constexpr (!inPlace)
        {
            f[cid] = ConstArrayPtr{cur, actualLen};
            cur += actualLen;
        }
        else
            f[cid].size = actualLen;
        dft<true>(ConstArrayPtr{tmpd, n}, const_cast<Number *>(f[cid].ptr), lgn);
    }
    void conquer(const ConstArrayPtr v, const unsigned int lgn)
    {
        const unsigned int n = 1u << lgn;
        {
            for (unsigned int i = 0; i < v.size; ++i)
            {
                cur[0] = v.ptr[i];
                f[n | i] = ConstArrayPtr{cur, 1};
                ++cur;
            }
            if (v.size < n)
            {
                cur[0] = Number{0};
                const unsigned int cid = v.size | n;
                f[cid] = ConstArrayPtr{cur, 1};
                ++cur;
                if (cid + 1 < (n << 1))
                    std::fill(f + cid + 1, f + (n << 1), f[cid]);
            }
        }
        for (unsigned int cid = n - 1; cid > 1; --cid)
            merge<false>(cid);
        merge<true>(1);
    }

    void interpolation(const Number x[], const Number y[], ArrayPtr &dest, const unsigned int n)
    {
        static Number memDx[maxn + 10], v[maxn + 10];
        if (n == 1)
        {
            dest.ptr[0] = y[0];
            dest.size = 1;
            return;
        }
        const unsigned int lgn = ceilLg[n];
        cur = mem;
        Product::product<false>(
            ConstArrayPtr{x, n}, lgn, prod, cur,
            [](Number *a, Number v) { a[0] = -v; a[1] = 1; });
        {
            ArrayPtr dx{memDx};
            derivative(prod[1], dx);
            Evaluation::evaluation(dx, ConstArrayPtr{x, n}, v);
            getModularInv(ConstArrayPtr{v, n}, v);
            for (unsigned int i = 0; i < n; ++i)
                v[i] *= y[i];
        }
        f[1] = dest;
        conquer(ConstArrayPtr{v, n}, lgn);
        dest.size = f[1].size;
    }
} // namespace Interpolation
```