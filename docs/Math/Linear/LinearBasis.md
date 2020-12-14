# LinearBasis

## Maximum xor sum

``` cpp
class LinearBasis
{
public:
    void insert(unsigned long long x)
    {
        for (int i = maxl; i >= 0 && x; --i)
        {
            if (!(x & (1ull << i)))
                continue;
            if (a[i])
                x ^= a[i];
            else
            {
                for (unsigned int j = 0; j < i; ++j)
                    if (x & (1ull << j))
                        x ^= a[j];
                for (unsigned int j = i + 1; j <= maxl; ++j)
                    if (a[j] & (1ull << i))
                        a[j] ^= x;
                a[i] = x;
                break;
            }
        }
    }
    unsigned long long max() const
    {
        unsigned long long ret = 0;
        for (unsigned int i = 0; i <= maxl; ++i)
            ret ^= a[i];
        return ret;
    }

private:
    unsigned long long a[maxl + 1] = {};
};
```

## kth xor sum

``` cpp
class LinearBasis
{
public:
    void insert(unsigned long long x);
    void init();
    unsigned long long kth(unsigned long long k);

    unsigned int size;
    bool zero = false;

private:
    unsigned long long a[maxl + 10], *ptr;
};

void LinearBasis::insert(unsigned long long x)
{
    for (int i = maxl; i >= 0; --i)
    {
        if (!(x & (1ull << i)))
            continue;
        if (a[i])
            x ^= a[i];
        else
        {
            for (int j = 0; j < i; ++j)
                if (x & (1ull << j))
                    x ^= a[j];
            for (int j = i + 1; j <= maxl; ++j)
                if (a[j] & (1ull << i))
                    a[j] ^= x;
            a[i] = x;
            break;
        }
    }
    zero = zero || !x;
}
void LinearBasis::init()
{
    ptr = remove(a, a + maxl + 1, 0);
    size = ptr - a;
}
unsigned long long LinearBasis::kth(unsigned long long k)
{
    k -= zero;
    unsigned long long ret = 0;
    for (int i = size - 1; i >= 0; --i)
        if (k & (1ull << i))
            ret ^= a[i];
    return ret;
}
```
