# Lyndon factorization

Lyndon 分解：串$S$的 Lyndon 分解记为$S=w_1w_2w_3\dots w_k$ ，其中所有$w_i$为简单串，并且他们的字典序按照非严格单减排序，即$w_1>w_2>w_3 \dots > w_k$。

## duval algorithm

```cpp
vector<unsigned int> duval(const char str[], const size_t len) // return right endpoints
{
    vector<unsigned int> ret;
    for (unsigned int i = 0; i < len;)
    {
        unsigned int ps2 = i, ps3 = i + 1;
        while (ps3 < len && str[ps2] <= str[ps3])
        {
            if (str[ps2] == str[ps3])
                ++ps2;
            else
                ps2 = i;
            ++ps3;
        }
        const unsigned int stp = ps3 - ps2;
        while (i <= ps2)
        {
            ret.push_back(i + stp);
            i += stp;
        }
    }
    return ret;
}
```

## Smallest cyclic shift