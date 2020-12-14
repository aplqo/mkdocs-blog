# Suffix Array
============

$sa[i]$: 所有后缀排序后第 $i$ 小的后缀的起始位置

$Rank[i]$: 从 $i$ 位置起始的后缀的排名

$hight[i]$: 第 $i$ 名后缀和它前一名后缀的 $LCP$

## build

``` cpp
unsigned int sa[maxn + 1], Rank[maxn + 1], hight[maxn + 1];

void radixSort(const size_t l, unsigned int rk[], unsigned int tp[])
{
    static unsigned int buk[maxn + 1];
    memset(buk, 0, sizeof(buk));
    for (unsigned int i = 1; i <= l; ++i)
        ++buk[rk[tp[i]]];
    partial_sum(buk, buk + max<size_t>(l, 26) + 1, buk);
    for (unsigned int i = l; i; --i)
        sa[buk[rk[tp[i]]]--] = tp[i];
}
void buildSA(const char str[], const size_t l)
{
    static unsigned int tmp[maxn + 1];
    unsigned int *rk = Rank, *tp = tmp;
    for (unsigned int i = 1; i <= l; ++i)
    {
        rk[i] = str[i] + 1 - 'a';
        tp[i] = i;
    }
    radixSort(l, rk, tp);
    for (unsigned int i = 1, p = 0; p < l; i <<= 1)
    {
        {
            unsigned int e = 1;
            for (unsigned int j = l + 1 - i; j <= l; ++j)
                tp[e++] = j;
            for (unsigned int j = 1; j <= l; ++j)
                if (sa[j] > i)
                    tp[e++] = sa[j] - i;
        }
        radixSort(l, rk, tp);
        swap(rk, tp);
        rk[sa[1]] = p = 1;
        for (unsigned int j = 2; j <= l; ++j)
            if (tp[sa[j]] == tp[sa[j - 1]] && tp[sa[j] + i] == tp[sa[j - 1] + i])
                rk[sa[j]] = p;
            else
                rk[sa[j]] = ++p;
    }
    for (unsigned int i = 1; i <= l; ++i)
        Rank[sa[i]] = i;
    for (unsigned int i = 1, k = 0; i <= l; ++i)
    {
        if (k)
            --k;
        while (str[i + k] == str[sa[rk[i] - 1] + k])
            ++k;
        hight[rk[i]] = k;
    }
}
```
