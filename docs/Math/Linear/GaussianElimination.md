# Gaussian Elimination

时间复杂度： $\text{O}(n^3)$

``` cpp
real equ[maxn][maxn];

void gauss(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        if (equ[i][i] >= 0 && equ[i][i] <= 0)
        {
            for (unsigned int j = i + 1; j < n; ++j)
                if (equ[j][i] != 0)
                {
                    swap_ranges(equ[i], equ[i] + n + 1, equ[j]);
                    break;
                }
        }
        for (unsigned int j = i + 1; j < n; ++j)
        {
            if (equ[j][i] >= 0 && equ[j][i] <= 0)
                continue;
            const real rat = equ[j][i] / equ[i][i];
            for (unsigned int k = i; k <= n; ++k)
                equ[j][k] -= equ[i][k] * rat;
        }
    }
    for (unsigned int i = n - 1; i > 0; --i)
    {
        equ[i][n] /= equ[i][i];
        for (unsigned int j = 0; j < i; ++j)
            equ[j][n] -= equ[j][i] * equ[i][n];
    }
    equ[0][n] /= equ[0][0];
}
```
