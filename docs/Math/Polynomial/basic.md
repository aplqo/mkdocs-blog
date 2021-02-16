# Utility functions

## Number

``` cpp
constexpr unsigned long long mod = 998244353, primitiveRoot = 3,
                             inv2 = 499122177;

class Number {
 public:
  constexpr Number() = default;
  constexpr Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline void operator x##=(const Number r)                       \
  {                                                               \
    val = ((val + mod) x r.val) % mod;                            \
  }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  inline Number operator-() const
  {
    Number ret;
    ret.val = val ? mod - val : 0;
    return ret;
  }
  inline friend std::ostream &operator<<(std::ostream &os, const Number v)
  {
    return os << v.val;
  }
  inline friend std::istream &operator>>(std::istream &is, Number &v)
  {
    return is >> v.val;
  }
  inline bool operator==(const unsigned long long v) const { return val == v; }

 private:
  unsigned long long val = 0;
};
Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number inverse(const Number a) { return quickPow(a, mod - 2); }

```

## Array pointer
``` cpp
template <class T>
struct Pointer {
  T *ptr;
  unsigned int size;

  template <class U>
  inline operator Pointer<U>() const
  {
    return Pointer<U>{ptr, size};
  }
  template <unsigned long long v = 0>
  void shrink()
  {
    while (size && ptr[size - 1] == v)
      --size;
  }

  inline friend ostream &operator<<(ostream &os, const Pointer p)
  {
    std::copy_n(p.ptr, p.size, std::ostream_iterator<Number>(os, " "));
    return os;
  }
};
typedef Pointer<Number> ArrayPtr;
typedef Pointer<const Number> ConstArrayPtr;
```

## modular inverse
``` cpp
static void initModularInv(const unsigned int n)
{
  inv[1] = 1;
  for (unsigned int i = 2; i <= n; ++i)
    inv[i] = -inv[mod % i] * (mod / i);
}
static void getModularInv(const ConstArrayPtr a, Number dest[])
{
  static Number prd[maxn + 10], iprd[maxn + 10];
  prd[0] = a.ptr[0];
  for (unsigned int i = 1; i < a.size; ++i)
    prd[i] = prd[i - 1] * a.ptr[i];
  iprd[a.size - 1] = inverse(prd[a.size - 1]);
  for (unsigned int i = a.size - 1; i; --i) {
    iprd[i - 1] = iprd[i] * a.ptr[i];
    dest[i] = iprd[i] * prd[i - 1];
  }
  dest[0] = iprd[0];
}
```

## Init
```cpp
static void initUnit(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i) {
    unit[0][i] = quickPow(primitiveRoot, (mod - 1) >> i);
    unit[1][i] = quickPow(unit[0][i], mod - 2);
  }
}
static void initRev(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i) {
    unsigned int *const crev = rev[i];
    for (unsigned int j = 0; j < i; ++j)
      crev[1u << j] = 1u << (i - 1 - j);
    const unsigned int n = 1u << i;
    for (unsigned int j = 3; j < n; ++j) {
      const unsigned int lb = j & -j;
      crev[j] = crev[j ^ lb] | crev[lb];
    }
  }
}
static void initLog(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i)
    std::fill(ceilLg + (1u << (i - 1)) + 1, ceilLg + (1u << i) + 1, i);
}
unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0;; ++i)
    if (n <= (1u << i)) return i;
  return 0;
}
```

## reverse copy
``` cpp
void reverseCopy(Number dest[], const ConstArrayPtr src, const unsigned int lgn)
{
  static constexpr Number zero;
  const unsigned int *const crev = rev[lgn];
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 0; i < src.size; ++i)
    dest[crev[i]] = src.ptr[i];
  for (unsigned int i = src.size; i < n; ++i)
    dest[crev[i]] = zero;
}
```
