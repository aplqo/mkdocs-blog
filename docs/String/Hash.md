# Hash

## parameters

1.  $Base$ ：⼤于字符集⼤⼩，质数。例如：$131$，$13131$...
1.  $P$: ⾮常⼤，需要⽤到逆元时必须是质数。例如：$1E9+7$,$1E9+9$,$2^32$,$2^64$,$23333333333333$......

## Conflict resolution

1.  换更⼤的质数（long long⾃然溢出，较快，理论上可以被卡）
1.  双质数（mod两个质数，都相等才判断，较慢）例如$1E9+7$和$1E9+9$

## example code

``` cpp
typedef uint_fast64_t hash_t;
const int maxn = 1e6;
const hash_t base = 131;

hash_t po[maxn + 10], has[maxn + 10];

inline void getPow(size_t n)
{
    po[0] = 1;
    for (size_t i = 1; i <= n; ++i)
        po[i] = po[i - 1] * base;
}
inline void getHash(const char* str, size_t len)
{
    for (size_t i = 1; i <= len; ++i)
        has[i] = has[i - 1] * base + str[i] - 'a';
}
inline hash_t getSub(unsigned int beg, size_t len)
{
    return has[beg - 1 + len] - has[beg - 1] * po[len];
}
```
