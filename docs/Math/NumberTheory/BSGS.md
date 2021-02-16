# BSGS

求解 $a^x \equiv b ( \mod p )$  

## BSGS

要求 $a$ 和 $p$ 互质。

``` cpp
unsigned long long mod;
unsigned long long quickPow(unsigned long long a, unsigned long long e);  // a^e
unsigned long long bsgs(unsigned long long b,
                        const unsigned long long n)  // a^x = n ( mod mod )
{
  const unsigned long long step = ceil(sqrt(mod));
  unordered_map<unsigned long long, unsigned long long> mem;
  {
    unsigned long long acc = n;
    mem.reserve(step);
    for (unsigned long long i = 0; i < step; ++i) {
      mem[acc] = i;
      acc = (acc * b) % mod;
    }
  }
  {
    unsigned long long acc = 1;
    const unsigned long long s = quickPow(b, step);
    for (unsigned long long i = 0; i < 2 * step; ++i) {
      if (auto it = mem.find(acc); it != mem.end() && i * step >= it->second)
        return i * step - it->second;
      acc = (acc * s) % mod;
    }
  }
  return ULLONG_MAX;  // no solution
}
```

## Extend

令 $d=\gcd (a,p)$

按照以下方法规约直到 $a$ $p$ 互质

$$\begin{aligned}
\begin{split}
    a^x &\equiv b (\mod p) \\
    a^x+yp&=b \\
    \frac{a}{d}a^{x-1}+\frac{yp}{d}&=\frac{b}{d} \\
    \frac{a^r}{d}a^{x-r} &\equiv \frac{b}{d} (\mod \frac{p}{d}) \\
\end{split}
\end{aligned}$$

如果某一步 $d \nmid b$ 则无解。

``` cpp
unsigned long long mod;
long long exgcd(const long long a, const long long b, long long& x,
                long long& y);
inline unsigned long long gcd(long long a, long long b);
inline unsigned long long inverse(long long y,
                                  long long p);  // y modular inverse p
unsigned long long quickPow(unsigned long long a, unsigned long long e);
unsigned long long bsgs(unsigned long long y, unsigned long long z,
                        const unsigned long long r)
{
  const unsigned long long step = sqrt(mod);
  multimap<unsigned long long, unsigned long long> mem;
  {
    unsigned long long acc = z % mod;
    for (unsigned long long i = 0; i < step; ++i) {
      mem.insert({acc, i});
      acc = (acc * y) % mod;
    }
  }
  {
    const unsigned long long s = quickPow(y, step);
    unsigned long long acc = 1 % mod;
    for (unsigned long long i = 0; i < 2 * step; ++i) {
      {
        const auto it = mem.equal_range(acc);
        bool found = false;
        unsigned long long ret = ULLONG_MAX;
        for (auto j = it.first; j != it.second; ++j)
          if (i * step >= j->second && quickPow(y, i * step - j->second) == z) {
            ret = min(ret, i * step + r - j->second);
            found = true;
          }
        if (found) return ret;
      }
      acc = (acc * s) % mod;
    }
  }
  return ULLONG_MAX;
}
unsigned long long exbsgs(unsigned long long y, unsigned long long z)
{
  unsigned long long r = 0, g = gcd(y, mod);
  while (true) {
    if (g == 1 || z == 1) break;
    mod /= g;
    if (z % g) return ULLONG_MAX;
    z = (z / g) % mod;
    ++r;
    z = (z * inverse(y / g, mod)) % mod;
    g = gcd(y, mod);
  }
  return bsgs(y, z, r);
}
```
