# divide

```cpp
void divide(const ConstArrayPtr a, const ConstArrayPtr b, ArrayPtr &div,
            ArrayPtr &mod)
{
  static Number tmp[maxn + 10], rev[maxn + 10];
  if (a.size < b.size) {
    div.size = 0;
    mod.size = a.size;
    if (a.ptr == mod.ptr) return;
    std::copy(a.ptr, a.ptr + a.size, mod.ptr);
    return;
  }
  const unsigned int dsize = a.size + 1 - b.size;
  ArrayPtr iv{tmp, dsize + 1};
  {
    std::reverse_copy(b.ptr, b.ptr + b.size, rev);
    inverse(ConstArrayPtr{rev, b.size}, iv);
  }
  std::reverse_copy(a.ptr, a.ptr + a.size, rev);
  ntt(ConstArrayPtr{rev, std::min(dsize + 1, a.size)}, iv, div);
  div.size = dsize;
  std::reverse(div.ptr, div.ptr + dsize);

  {
    ArrayPtr pTmp{tmp};
    ntt(b, div, pTmp);
    mod.size = b.size - 1;
    for (unsigned int i = 0; i < mod.size; ++i)
      mod.ptr[i] = a.ptr[i] - tmp[i];
  }
}
```
