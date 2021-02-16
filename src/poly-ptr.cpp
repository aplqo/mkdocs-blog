// Polynomial template test (pointer version)
#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxln = 18, maxn = 1u << maxln;
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

  inline friend std::ostream &operator<<(std::ostream &os, const Pointer p)
  {
    std::copy_n(p.ptr, p.size, std::ostream_iterator<Number>(os, " "));
    return os;
  }
};
typedef Pointer<Number> ArrayPtr;
typedef Pointer<const Number> ConstArrayPtr;

Number unit[2][maxln + 10], inv[maxn + 10];
unsigned int ceilLg[maxn + 10], rev[maxln + 10][maxn + 10];

template <class... Args>
void printP(Args... args)
{
  (clog << ... << args) << endl;
}

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
static void initModularInv(const unsigned int n)
{
  inv[1] = 1;
  for (unsigned int i = 2; i <= n; ++i)
    inv[i] = -inv[mod % i] * (mod / i);
}
unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0;; ++i)
    if (n <= (1u << i)) return i;
  return 0;
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
static void init(const unsigned int n)
{
  const unsigned int lgn = Log2(n) + 2;
  initRev(lgn);
  initUnit(lgn);
  initLog(lgn);
}

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

inline void updateInverse(const ConstArrayPtr &a, const Number old[],
                          Number dest[], const unsigned int n)
{
  convolution(a, ConstArrayPtr{old, n >> 1}, ArrayPtr{dest, n << 1},
              [](Number a, Number b) { return b * (2 - a * b); });
}
void inverse(const ConstArrayPtr a, const ArrayPtr dest)
{
  Number *const pDest = dest.ptr;
  pDest[0] = inverse(a.ptr[0]);
  if (dest.size == 1) return;
  const unsigned int n = 1u << ceilLg[dest.size],
                     lim = std::max(2u, 1u << ceilLg[a.size]);
  for (unsigned int i = 2; i < lim; i <<= 1)
    updateInverse(ConstArrayPtr{a.ptr, i}, pDest, pDest, i);
  for (unsigned int i = lim; i <= n; i <<= 1)
    updateInverse(a, pDest, pDest, i);
}
void sqrt(const ConstArrayPtr a, const ArrayPtr dest)
{
  static Number memInv[maxn + 10], memTinv[maxn + 10], tmpMem[maxn + 10];
  static const auto updateSqrt = [](const ConstArrayPtr a, Number dest[],
                                    const unsigned int n) {
    updateInverse(ConstArrayPtr{dest, n >> 1}, memInv, memTinv, n);
    ArrayPtr tmp{tmpMem};
    ntt(ConstArrayPtr{memTinv, n}, a, tmp);
    {
      const unsigned int half = n >> 1;
      for (unsigned int j = 0; j < half; ++j)
        dest[j] = inv2 * (dest[j] + tmpMem[j]);
      for (unsigned int j = half; j < n; ++j)
        dest[j] = inv2 * tmpMem[j];
    }
    updateInverse(ConstArrayPtr{dest, n}, memInv, memInv, n);
  };
  const unsigned int lgn = ceilLg[dest.size], lim = ceilLg[a.size];
  memInv[0] = dest.ptr[0] = 1;
  if (dest.size == 1) return;
  for (unsigned int i = 1; i < lim; ++i)
    updateSqrt(ConstArrayPtr{a.ptr, 1u << i}, dest.ptr, 1u << i);
  for (unsigned int i = lim; i <= lgn; ++i)
    updateSqrt(a, dest.ptr, 1u << i);
}
void divide(const ConstArrayPtr a, const ConstArrayPtr b, ArrayPtr &div,
            ArrayPtr &mod)
{
  static Number tmp[maxn + 10], rev[maxn + 10];
  if (a.size < b.size) {
    div.size = 0;
    mod.size = a.size;
    if (a.ptr == mod.ptr) return;
    std::copy(a.ptr, a.ptr + a.size, mod.ptr);
    return;
  }
  const unsigned int dsize = a.size + 1 - b.size;
  ArrayPtr iv{tmp, dsize + 1};
  {
    std::reverse_copy(b.ptr, b.ptr + b.size, rev);
    inverse(ConstArrayPtr{rev, b.size}, iv);
  }
  std::reverse_copy(a.ptr, a.ptr + a.size, rev);
  ntt(ConstArrayPtr{rev, std::min(dsize + 1, a.size)}, iv, div);
  div.size = dsize;
  std::reverse(div.ptr, div.ptr + dsize);

  {
    ArrayPtr pTmp{tmp};
    ntt(b, div, pTmp);
    mod.size = b.size - 1;
    for (unsigned int i = 0; i < mod.size; ++i)
      mod.ptr[i] = a.ptr[i] - tmp[i];
  }
}
inline void derivative(const ConstArrayPtr a, ArrayPtr &dest)
{
  for (unsigned int i = 0; i + 1 < a.size; ++i)
    dest.ptr[i] = a.ptr[i + 1] * (i + 1);
  dest.size = a.size - 1;
}
inline void integral(const ConstArrayPtr a, ArrayPtr &dest)
{
  for (unsigned int i = 1; i <= a.size; ++i)
    dest.ptr[i] = a.ptr[i - 1] * inv[i];
  dest.ptr[0] = 0;
  dest.size = a.size + 1;
}
void ln(const ConstArrayPtr a, ArrayPtr dest)
{
  static Number tpMem[maxn + 10], ivMem[maxn + 10];
  ArrayPtr tp{tpMem}, iv{ivMem, dest.size};
  derivative(a, tp);
  inverse(a, iv);
  ntt(tp, iv, iv);
  iv.size = dest.size;
  integral(iv, dest);
}
void exp(const ConstArrayPtr a, const ArrayPtr dest)
{
  static Number tmpMem[maxn + 10];
  dest.ptr[0] = 1;
  const unsigned int lgn = ceilLg[dest.size];
  ArrayPtr pDest{dest.ptr};
  for (unsigned int i = 1; i <= lgn; ++i) {
    const unsigned int n = 1u << i;
    ArrayPtr tmp{tmpMem, n};
    ln(ConstArrayPtr{dest.ptr, 1u << (i - 1)}, tmp);
    {
      const unsigned int lim = std::min(a.size, n);
      for (unsigned int i = 0; i < lim; ++i)
        tmpMem[i] = a.ptr[i] - tmpMem[i];
      for (unsigned int i = lim; i < n; ++i)
        tmpMem[i] = -tmpMem[i];
    }
    tmpMem[0] += 1;
    ntt(tmp, ConstArrayPtr{dest.ptr, n >> 1}, pDest);
  }
}
Number eval(const ConstArrayPtr pol, const Number x)
{
  Number ret = pol.ptr[pol.size - 1];
  if (pol.size > 1) {
    for (unsigned int i = pol.size - 1; i; --i)
      ret = ret * x + pol.ptr[i - 1];
  }
  return ret;
}

namespace Product {
template <bool inPlace>
inline void multiply(ConstArrayPtr &now, const ConstArrayPtr *left,
                     Number *&cur)
{
  ArrayPtr tp;
  if (inPlace)
    tp.ptr = const_cast<Number *>(now.ptr);
  else
    tp.ptr = cur;
  ntt(*left, *(left + 1), tp);
  if (!inPlace) cur += tp.size;
  now = tp;
}
template <bool inPlace, class Func>
void product(const ConstArrayPtr a, const unsigned int lgn,
             ConstArrayPtr *const prod, Number *&cur, Func ini)
{
  const unsigned int msk = 1u << lgn;
  {
    static const Number zero;
    for (unsigned int i = 0; i < a.size; ++i) {
      ini(cur, a.ptr[i]);
      prod[i | msk] = ConstArrayPtr{cur, 2};
      cur += 2;
    }
    if (a.size < msk) {
      *cur = 1;
      const unsigned int cid = a.size | msk;
      prod[cid] = ConstArrayPtr{cur, 1};
      ++cur;
      if (cid + 1 < (msk << 1))
        fill(prod + cid + 1, prod + (msk << 1), prod[cid]);
    }
  }
  for (unsigned int cid = msk - 1; cid > 1; --cid) {
    const unsigned int left = cid << 1, right = left | 1;
    if (prod[right].size == 1 && prod[right].ptr[0] == 1)
      prod[cid] = prod[left];
    else
      multiply<false>(prod[cid], prod + left, cur);
  }
  multiply<inPlace>(prod[1], prod + 2, cur);
}

template <class Func>
inline void product(const ConstArrayPtr &a, ArrayPtr &dest, Func ini)
{
  if (a.size == 1) {
    ini(dest.ptr, a.ptr[0]);
    dest.size = 2;
    return;
  }
  static Number mem[maxn * maxln * 4 + 10];
  static ConstArrayPtr prod[maxn * 4 + 10];
  prod[1] = ConstArrayPtr{dest.ptr, 0};
  Number *cur = mem;
  product<true>(a, ceilLg[a.size], prod, cur, ini);
  dest.size = prod[1].size;
}
}  // namespace Product
namespace Evaluation {
Number mem[maxn * maxln * 4 * 2 + 10], *cur = mem;
ConstArrayPtr t[maxn * 4 + 10], prod[maxn * 4 + 10];

ConstArrayPtr transMultiply(const Number vx[], const ConstArrayPtr &b,
                            const unsigned int lgn)
{
  static Number tmpb[maxn + 10];
  dft<false>(b, tmpb, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] *= vx[i];
  dft<false>(ConstArrayPtr{tmpb, n}, cur, lgn);
  return ConstArrayPtr{cur, n};
}
void conquer(Number *const dest, const unsigned int lgn)
{
  static unsigned int size[maxn + 10];
  const unsigned int n = 1u << lgn;
  size[1] = n;
  for (unsigned int cid = 1; cid < n; ++cid) {
    static Number tmpv[maxn + 10];
    const unsigned int left = cid << 1, right = left | 1;
    const unsigned int actualSize = size[cid] >> 1;
    const unsigned int childSize = t[cid].size +
                                   std::max(prod[left].size, prod[right].size) -
                                   1,
                       lgn = ceilLg[childSize];
    dft<true>(t[cid], tmpv, lgn);
    t[left] = transMultiply(tmpv, prod[right], lgn);
    t[left].size = size[left] = actualSize;
    cur += actualSize;
    t[right] = transMultiply(tmpv, prod[left], lgn);
    t[right].size = size[right] = actualSize;
    cur += actualSize;
  }
  const ConstArrayPtr *p = t + n;
  for (unsigned int i = 0; i < n; ++i, ++p)
    dest[i] = p->ptr[0];
}

void evaluation(const ConstArrayPtr &a, const ConstArrayPtr &pos, Number dest[])
{
  if (pos.size == 1) {
    dest[0] = eval(a, pos.ptr[0]);
    return;
  }
  else if (a.size == 1) {
    std::fill(dest, dest + pos.size, a.ptr[0]);
    return;
  }
  cur = mem;
  const unsigned int len = std::max(a.size, pos.size), lgn = ceilLg[len],
                     n = 1u << lgn;
  Product::product<false>(pos, lgn, prod, cur, [](Number *cur, Number v) {
    cur[0] = 1;
    cur[1] = -v;
  });
  {
    static Number tmp[maxn + 10];
    ArrayPtr iv{tmp, std::max(n, prod[1].size)}, pv{cur, 0};
    inverse(prod[1], iv);
    ntt<false, true, false>(iv, a, pv);
    pv.size = n;
    t[1] = pv;
    cur += n;
  }
  conquer(dest, lgn);
}
}  // namespace Evaluation
namespace Interpolation {
Number mem[maxn * maxln * 4 * 2 + 10], *cur = mem;
ConstArrayPtr f[maxn * 4 + 10], prod[maxn * 4 + 10];

template <bool inPlace>
inline void merge(const unsigned int cid)
{
  static Number tmpa[maxn + 10], tmpb[maxn + 10], tmpc[maxn + 10],
      tmpd[maxn + 10];
  const unsigned int left = cid << 1, right = left | 1;
  if (f[right].size == 1 && f[right].ptr[0] == 0 && prod[right].size == 1 &&
      prod[right].ptr[0] == 1) {
    if constexpr (inPlace) {
      std::copy(f[left].ptr, f[left].ptr + f[left].size,
                const_cast<Number *>(f[cid].ptr));
      f[cid].ptr = f[left].ptr;
    }
    else
      f[cid] = f[left];
    return;
  }
  const unsigned int actualLen =
      max(f[left].size + prod[right].size, f[right].size + prod[left].size) - 1;
  const unsigned int lgn = ceilLg[actualLen], n = 1u << lgn;
  dft<false>(f[left], tmpa, lgn);
  dft<false>(prod[left], tmpb, lgn);
  dft<false>(f[right], tmpc, lgn);
  dft<false>(prod[right], tmpd, lgn);
  for (unsigned int i = 0; i < n; ++i)
    tmpd[i] = tmpa[i] * tmpd[i] + tmpb[i] * tmpc[i];
  if constexpr (!inPlace) {
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
    for (unsigned int i = 0; i < v.size; ++i) {
      cur[0] = v.ptr[i];
      f[n | i] = ConstArrayPtr{cur, 1};
      ++cur;
    }
    if (v.size < n) {
      cur[0] = Number{0};
      const unsigned int cid = v.size | n;
      f[cid] = ConstArrayPtr{cur, 1};
      ++cur;
      if (cid + 1 < (n << 1)) std::fill(f + cid + 1, f + (n << 1), f[cid]);
    }
  }
  for (unsigned int cid = n - 1; cid > 1; --cid)
    merge<false>(cid);
  merge<true>(1);
}

void interpolation(const Number x[], const Number y[], ArrayPtr &dest,
                   const unsigned int n)
{
  static Number memDx[maxn + 10], v[maxn + 10];
  if (n == 1) {
    dest.ptr[0] = y[0];
    dest.size = 1;
    return;
  }
  const unsigned int lgn = ceilLg[n];
  cur = mem;
  Product::product<false>(ConstArrayPtr{x, n}, lgn, prod, cur,
                          [](Number *a, Number v) {
                            a[0] = -v;
                            a[1] = 1;
                          });
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
}  // namespace Interpolation

int main()
{
  std::ios_base::sync_with_stdio(false);
  unsigned int n;
  std::cin >> n;
  static Number x[maxn + 10], y[maxn + 10], ans[maxn + 10];
  for (unsigned int i = 0; i < n; ++i)
    std::cin >> x[i] >> y[i];
  init(n);
  ArrayPtr dest{ans};
  Interpolation::interpolation(x, y, dest, n);
  for (unsigned int i = 0; i < dest.size; ++i)
    std::cout << dest.ptr[i] << " ";
  for (unsigned int i = dest.size; i < n; ++i)
    std::cout << "0 ";
  std::cout.put('\n');
  return 0;
}