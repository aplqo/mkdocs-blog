# derivate and integral

## derivate
```cpp
inline void derivative(const ConstArrayPtr a, ArrayPtr &dest)
{
  for (unsigned int i = 0; i + 1 < a.size; ++i)
    dest.ptr[i] = a.ptr[i + 1] * (i + 1);
  dest.size = a.size - 1;
}
```

## integral
```cpp
inline void integral(const ConstArrayPtr a, ArrayPtr &dest)
{
  for (unsigned int i = 1; i <= a.size; ++i)
    dest.ptr[i] = a.ptr[i - 1] * inv[i];
  dest.ptr[0] = 0;
  dest.size = a.size + 1;
}
```
