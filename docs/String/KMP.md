# KMP

$nxt[i]$表示第i个位置最⻓前缀等于后缀的⻓度

example code

``` cpp
const int maxn = 1e6;

char s1[maxn + 10], s2[maxn + 10];
unsigned int l1, l2, nxt[maxn + 10];

void pre()
{
    unsigned int j = 0;
    for (unsigned int i = 1; i <= l2; ++i)
    {
        while (j && s2[i + 1] != s2[j + 1])
            j = nxt[j];
        if (s2[j + 1] == s2[i + 1])
            ++j;
        nxt[i + 1] = j;
    }
}
void kmp()
{
    unsigned int k = 0;
    for (unsigned int i = 1; i <= l1; ++i)
    {
        while (k && s1[i] != s2[k + 1])
            k = nxt[k];
        if (s1[i] == s2[k + 1])
            ++k;
        if (k == l2)
        {
            cout << i - k + 1 << endl;
            k = nxt[k];
        }
    }
}
```
