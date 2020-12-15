# Smallest cyclic shift

字符串$S$的最小表示为与$S$循环同构的所有字符串中字典序最小的字符串

## Solution 1

```cpp
unsigned int minRepresent(unsigned int str[], const unsigned int len)
{
    copy(str, str + len, str + len);
    unsigned int p1 = 0, p2 = 1, l = 0;
    while (p1 < len && p2 < len && l < len)
    {
        if (str[p1 + l] == str[p2 + l])
            ++l;
        else
        {
            if (str[p1 + l] > str[p2 + l])
                p1 += l + 1;
            else
                p2 += l + 1;
            p1 += p1 == p2;
            l = 0;
        }
    }
    return min(p1, p2);
}
```

## Solution 2

Solve with [lyndon factorization](/String/Lyndon/#smallest-cyclic-shift)