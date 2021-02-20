# Divide optimization


[full code](./../../src/divide.cpp)

```cpp
template <class T>
inline constexpr T highMultiplyAdd(const T a, const T b, const T c); // high part of a*b+c
constexpr static inline unsigned int Log2(Integer x);

struct Magic {
  Integer multiply, increase;
  unsigned int preShift, postShift;

  static constexpr Magic init(const Integer mod, const unsigned int modBit)
  {
    constexpr unsigned int bits = std::numeric_limits<Integer>::digits;

    const unsigned int lgm = Log2(mod), extraExponent = bits - modBit;
    const Integer initPow2 = Integer(1) << (bits - 1);
    bool hasDown = false;
    unsigned int exponent;
    Integer downMultiply, downShift;
    Integer quotient = initPow2 / mod, reminder = initPow2 % mod;

    for (exponent = 0;; ++exponent) {
      if (reminder >= mod - reminder) {
        quotient = (quotient << 1) + 1;
        reminder = (reminder << 1) - mod;
      }
      else {
        quotient <<= 1;
        reminder <<= 1;
      }

      const Integer tmp = Integer(1) << (exponent + extraExponent);
      if (exponent + extraExponent >= lgm || (mod - reminder) <= tmp) break;
      if (!hasDown && reminder <= tmp) {
        hasDown = true;
        downMultiply = quotient;
        downShift = exponent;
      }
    }

    if (exponent < lgm)
      return Magic{.multiply = quotient + 1,
                   .increase = 0,
                   .preShift = 0,
                   .postShift = exponent};
    else if (mod & 0x01)
      return Magic{.multiply = downMultiply,
                   .increase = downMultiply,
                   .preShift = 0,
                   .postShift = downShift};
    else {
      Integer shiftMod = mod;
      unsigned int shift = 0;
      while (!(shiftMod & 0x01)) {
        shiftMod >>= 1;
        ++shift;
      }

      Magic mg = Magic::init(shiftMod, modBit - shift);
      mg.preShift = shift;
      return mg;
    }
  }
};
inline static constexpr Integer divide(const Integer val, const Magic p)
{
  return highMultiplyAdd<Integer>(val >> p.preShift, p.multiply, p.increase) >>
         p.postShift;
}
inline static constexpr Integer mod(const Integer val, const Integer m,
                                    const Magic p)
{
  const Integer q = divide(val, p);
  return val - q * m;
}
```