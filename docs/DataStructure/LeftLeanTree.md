# Left leaning tree

是一种可并堆，具有堆性质，可以快速合并。

``` cpp
struct Node {
  inline void pop();
  int val;
  unsigned int dist = 1;
  Node *lc = nullptr, *rc = nullptr;
} nodes[maxn + 1];

void Node::pushup()
{
  if (rc && (!lc || lc->dist < rc->dist)) swap(lc, rc);
  dist = (rc ? rc->dist : 0) + 1;
}
Node* merge(Node* x, Node* y)
{
  if (!x || !y) return x ? x : y;
  if (x->val > y->val) swap(x, y);
  x->rc = merge(x->rc, y);
  x->pushup();
  return x;
}
Node* pop(Node* x)
{
  Node* c = merge(x->lc, x->rc);
  if (c) c->pushup();
  return c;
}
```
