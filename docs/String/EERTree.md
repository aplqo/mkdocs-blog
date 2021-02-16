# EER Tree

## build

For a string $s$, the number of essentially distinct palindromic substrings is at most $|s|$

```cpp
struct EERNode {
  EERNode *link, *next[maxc];
  int len;
} nodes[maxn + 10]{}, *cur = nodes;
EERNode oddRoot{&oddRoot, {}, -1}, evenRoot{&oddRoot, {}, 0}, *last = &evenRoot;
unsigned int str[maxn + 10] = {UINT_MAX};

void insertChar(const unsigned int ch, const int pos)
{
  EERNode* fa = last;
  str[pos] = ch;
  while (str[pos - fa->len - 1] != ch)
    fa = fa->link;
  if (!fa->next[ch]) {
    EERNode* const ptr = cur++;
    EERNode* lk = fa->link;
    while (str[pos - lk->len - 1] != ch)
      lk = lk->link;
    lk = lk->next[ch] ? lk->next[ch] : &evenRoot;
    ptr->link = lk;
    ptr->len = fa->len + 2;
    fa->next[ch] = ptr;
  }
  last = fa->next[ch];
}
```

## application

