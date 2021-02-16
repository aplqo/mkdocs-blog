# Du jiao sieve

计算$s(i)=\sum_{i=1}^{n}f(i)$ ($f(i)$为积性函数)     
直接计算的时间复杂度为$O(n^{\frac{3}{4}})$ 。考虑先线性筛预处理出前$n^{\frac{2}{3}}$项，剩余部分的时间复杂度为$O(n^{\frac{2}{3}})$

$$
\begin{align}
\sum_{i=1}^{n}(f*g)(i)&=\sum_{i=1}^{n}\sum_{d \mid i}f(d)g(\frac{i}{d}) \\
    &=\sum_{i=1}^{n}\sum_{j=1}^{\lfloor \frac{n}{i} \rfloor} g(i)f(j)\\
    &=\sum_{i=1}^{n}g(i)*s(\lfloor \frac{n}{i} \rfloor)
\end{align}
$$  

$g(1)s(i)=\sum_{i=1}^{n}(f*g)(i)-\sum_{i=2}^{n}g(i)s(\lfloor \frac{n}{i} \rfloor)$

## Example code

计算$\sum \phi(i)$,$\sum \mu(i)$

```cpp
// @returns (mu,phi)
static pair<long long, unsigned long long> dujiao(const unsigned int n)
{
  static unordered_map<unsigned int, unsigned long long> memPhi;
  static unordered_map<unsigned int, long long> memMu;
  if (n <= block) return make_pair(mu[n], phi[n]);
  if (const auto it = memPhi.find(n); it != memPhi.end())
    return make_pair(memMu.at(n), it->second);
  long long m = 1;
  unsigned long long p = n * static_cast<unsigned long long>(n + 1) >> 1;
  for (unsigned int i = 2; i <= n;) {
    const unsigned int cv = n / i, nxt = n / cv + 1;
    const auto [nm, np] = dujiao(cv);
    m -= nm * (nxt - i);
    p -= np * (nxt - i);
    i = nxt;
  }
  memPhi[n] = p;
  memMu[n] = m;
  return make_pair(m, p);
}
```
