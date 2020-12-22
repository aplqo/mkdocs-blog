# Lagrange interpolation

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