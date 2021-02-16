# Aho-Corasick automaton

example code

``` cpp
const int maxn = 1e6;

unsigned int ch[maxn + 10][26], b[maxn + 10], fail[maxn + 10];
unsigned int ans = 0;

void add(const char* str)
{
  static unsigned int cur = 0;
  size_t l = strlen(str);
  int k = 0;
  for (size_t i = 0; i < l; ++i) {
    int c = str[i] - 'a';
    if (!ch[k][c]) ch[k][c] = ++cur;
    k = ch[k][c];
  }
  ++b[k];
}
void make()
{
  queue<unsigned int> q;
  for (unsigned int i = 0; i < 26; ++i)
    if (ch[0][i]) q.push(ch[0][i]);
  while (!q.empty()) {
    int cur = q.front();
    q.pop();
    for (unsigned int i = 0; i < 26; ++i) {
      if (!ch[cur][i])
        ch[cur][i] = ch[fail[cur]][i];
      else {
        fail[ch[cur][i]] = ch[fail[cur]][i];
        q.push(ch[cur][i]);
      }
    }
  }
}
void find(const char* str)
{
  size_t l = strlen(str);
  unsigned int k = 0;
  for (size_t i = 0; i < l; ++i) {
    k = ch[k][str[i] - 'a'];
    ans += b[k];
    b[k] = 0;
  }
}
```
