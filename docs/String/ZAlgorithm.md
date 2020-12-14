# Z Algorithm

$z[i]$ is the length of the longest common prefix between $s$ and the
suffix of $s$ starting at $i$

``` cpp
void ZAlgorithm(const char str[], unsigned int z[], const unsigned int len)
{
    unsigned int l = 0, r = 0;
    for (unsigned int i = 1; i < len; ++i)
    {
        if (i < r)
            z[i] = z[i - l];
        if (i + z[i] < r)
            continue;
        z[i] = min(z[i], r - i);
        while (i + z[i] < len && str[z[i]] == str[i + z[i]])
            ++z[i];
        if (i + z[i] > r)
        {
            r = i + z[i];
            l = i;
        }
    }
}
```
