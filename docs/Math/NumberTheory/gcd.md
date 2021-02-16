# GCD

## Euclidean Algorithm

``` cpp
unsigned int gcd(unsigned int a, unsigned int b)  // a > b
{
  unsigned int r = a % b;
  while (r) {
    a = b;
    b = r;
    r = a % b;
  }
  return b;
}
```

## exgcd

扩展欧几里得算法可求出 $ax+by=\gcd (a,b)$ 的一组特解。

$$
\begin{aligned}
\begin{split}
    Ax+By&=\gcd (A,B) \\
    ax+by&=\gcd (B,A \% B) \\
\end{split}
\end{aligned}
$$

$$ 
\begin{aligned}
\begin{equation}
    \left\{ \begin{aligned}
        A&=\left\lfloor \frac{A}{B} \right\rfloor a+b \\
        B&=a
    \end{aligned} \right. \implies
    \left\{ \begin{aligned}
        A- \left\lfloor \frac{A}{B} \right\rfloor B &=b \\
        B&=a
    \end{aligned} \right.
\end{equation}
\end{aligned}
$$

$$
\begin{aligned}
\begin{split}
    \therefore \gcd (A,B)&=ax+by \\
    &=Bx+\left( A- \left\lfloor \frac{A}{B} \right\rfloor B \right) y\\
    &=Ay+B \left( x- \left\lfloor \frac{A}{B} \right\rfloor y\right)
\end{split}
\end{aligned}
$$

通解

$$
\begin{aligned}
\begin{equation}
    \left\{ \begin{aligned}
        x&=x_0+k \frac{b}{\gcd (a,b)} \\
        y&=y_0-k\frac{a}{\gcd (a,b)}
    \end{aligned} \right.
\end{equation}
\end{aligned}
$$

``` cpp
typedef long long num_t;

num_t exgcd(num_t a, num_t b, num_t& x, num_t& y)
{
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  num_t g = exgcd(b, a % b, x, y);
  num_t t = x;
  x = y;
  y = t - (a / b) * y;
  return g;
}
```

模板元编程实现编译期计算：

``` cpp
typedef int num_t;

template <num_t a, num_t b>
class inverse {
 public:
  enum : num_t {
    X = inverse<b, a % b>::Y,
    Y = inverse<b, a % b>::X - (a / b) * inverse<b, a % b>::Y
  };
};
template <num_t a>
class inverse<a, 0> {
 public:
  enum { X = 1, Y = 0 };
};
```
