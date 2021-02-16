# Prime

## Sieve Of Euler

``` cpp
const int maxn = 1e8;

unsigned int primes[maxn + 10], mindiv[maxn + 10], *pc = primes;

inline void euler(unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pc++) = i;
    }
    for (unsigned int* j = primes; j < pc && *j <= mindiv[i] && *j * i <= n;
         ++j)
      mindiv[*j * i] = *j;
  }
}
```

## Miller rabin primality test

| Maximum $n$                             | Selected number                               |
| --------------------------------------- | --------------------------------------------- |
| $2,047$                                 | $2$                                           |
| $1,373,653$                             | $2,3$                                         |
| $9,080,191$                             | $31,73$                                       |
| $25,326,001$                            | $2,3,5$                                       |
| $3,215,031,751$                         | $2,3,5,7$                                     |
| $4,759,123,141$                         | $2,7,61$                                      |
| $1,122,004,669,633$                     | $2,13,23,1662803$                             |
| $2,152,302,898,747$                     | $2, 3, 5, 7,  11$                             |
| $3,474,749,660,383$                     | $2, 3, 5, 7, 11, 13$                          |
| $341,550,071,728,321$                   | $2, 3, 5, 7, 11, 13,17$                       |
| $3,825,123,056,546,413,051$             | $2, 3, 5, 7, 11,13,17,19,23$                  |
| $18,446,744,073,709,551,616 = 2^{64}$   | $2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31,37$   |

``` cpp
inline pair<unsigned int, unsigned long long> lowbit(unsigned long long n)
{
  unsigned int ret = 0;
  while (!(n & 0x01)) {
    ++ret;
    n >>= 1;
  }
  return make_pair(ret, n);
}
bool millerRabin(const unsigned long long n)  // return true if n is prime.
{
  static constexpr unsigned long long testA[] = {2,  3,  5,  7,  11, 13,
                                                 17, 19, 23, 29, 31, 37};
  const auto [r, d] = lowbit(n - 1);
  for (const unsigned long long a : testA) {
    if (a > n - 2) break;
    unsigned long long x = quickPow(a, d, n);
    if (x == 1 || x == n - 1) continue;
    for (unsigned int i = 1; i < r; ++i) {
      x = multiply(x, x, n);
      if (x == n - 1) goto End;
    }
    return false;
  End:;
  }
  return true;
}
```

## Pollard rho factor

``` cpp
inline unsigned long long getRandom(const unsigned int n)
{
  uniform_int_distribution<unsigned long long> dis(1, n);
  return dis(rnd);
}
unsigned long long pollardRho(const unsigned long long n)
{
  static const auto f = [](const unsigned long long x, const unsigned int c,
                           const unsigned long long m) {
    return (multiply(x, x, m) + c) % m;
  };
  const unsigned long long c = getRandom(n - 1);
  unsigned long long x = getRandom(n - 1), y = x;
  for (unsigned int i = 1; i; i <<= 1) {
    unsigned long long acc = 1;
    for (unsigned int j = 0; j < i; ++j) {
      y = f(y, c, n);
      acc = multiply(acc, (x > y ? x - y : y - x), n);
      if ((j & 0x7fu) == 0) {
        const unsigned long long g = gcd(acc, n);
        if (g > 1) return g;
      }
    }
    const unsigned long long g = gcd(acc, n);
    if (g > 1) return g;
    x = y;
  }
  return 1;
}
```
