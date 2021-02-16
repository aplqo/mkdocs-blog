# Sequence automaton

## build

Build time $\text{O}(n|\Sigma|)$

```cpp
class SeqAM {
 public:
  struct Node : public NodeBase<Node> {
  } nodes[maxn + 10];
  Node* root = nodes;

  void build(const char buf[], const size_t len);

 private:
  Node* cur = nodes + 1;
};

void SeqAM::build(const char buf[], const size_t len)
{
  Node* last[maxc]{};
  for (const char* i = buf + len - 1; i >= buf; --i) {
    Node* const ptr = cur++;
    ptr->id = ptr - nodes;
    copy(last, last + maxc, ptr->next);
    last[*i - 'a'] = ptr;
  }
  copy(last, last + maxc, root->next);
  root->id = 0;
}
```
