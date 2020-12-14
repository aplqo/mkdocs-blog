# manacher

求解⼀个字符串的最⻓回⽂⼦串⻓度。

在原字符串的每个相邻两个字符中间插入一个分隔符，同时在首尾也要添加一个分隔符，分隔符的要求是不在原串中出现，一般情况下可以用\#号。

example code

``` cpp
const int maxn = 11000000;

char str[(maxn + 10) * 2];
unsigned int rad[(maxn + 10) * 2];

void manacher(const char str[], unsigned int len)
{
    unsigned int r = 0, c = 0;
    rad[0] = -1;
    for (unsigned int i = 0; i < len; ++i)
    {
        unsigned int j = 0;
        if (i < r && rad[2 * c - i] + i - 1 < r)
        {
            rad[i] = rad[2 * c - i];
            continue;
        }
        else if (i < r)
        {
            j = r - i;
            rad[i] = r - i;
        }
        for (; i + j < len; ++j)
            if (str[i + j] == str[i - j])
            {
                r = i + j;
                ++rad[i];
            }
            else
                break;
        c = i;
    }
}
void read(char str[], unsigned int& len)
{
    len = 1;
    while (cin)
    {
        cin >> str[len];
        if (cin.fail())
            break;
        len += 2;
    }
}
```
