# Suffix Automaton

## Suffix Automaton

### build

```cpp
struct SAMNode
{
    SAMNode* link;
    unordered_map<unsigned int, SAMNode*> next;
    vector<SAMNode*> chl;
    unsigned int len;
} nodes[maxn * 3 + 10] {}, *cur = nodes, root {}, *last = &root;

void insertChar(const unsigned int c)
{
    SAMNode *const ptr = cur++, *from = last;
    new (ptr) SAMNode { nullptr, {}, {}, last->len + 1 };
    while (from && !from->next.count(c))
    {
        from->next[c] = ptr;
        from = from->link;
    }
    if (!from)
        ptr->link = &root;
    else
    {
        SAMNode* const fNxt = from->next[c];
        if (fNxt->len == from->len + 1)
            ptr->link = fNxt;
        else
        {
            SAMNode* const cloned = cur++;
            *cloned = *fNxt;
            cloned->len = from->len + 1;
            for (SAMNode* i = from; i; i = i->link)
            {
                const auto it = i->next.find(c);
                if (it != i->next.end() && it->second == fNxt)
                    it->second = cloned;
                else
                    break;
            }
            fNxt->link = ptr->link = cloned;
        }
    }
    last = ptr;
}
```

## Generic Suffix Automaton

Suffix Automaton process more than two strings.

### build

```cpp
struct SAMNode
{
    unordered_map<char, SAMNode*> next;
    SAMNode* link = nullptr;
    unsigned int len = 0;
} nodes[maxsn * 2 + 10], *cur = nodes, root {};

void insertTrie(const char* str)
{
    const size_t len = strlen(str);
    SAMNode* ptr = &root;
    for (const char* i = str; i < str + len; ++i)
    {
        auto it = ptr->next.find(*i);
        if (it == ptr->next.end())
            it = ptr->next.insert({ *i, cur++ }).first;
        ptr = it->second;
    }
}
void insertChar(SAMNode* const x, const char ch, SAMNode* const last)
{
    SAMNode* from = last->link;
    x->len = last->len + 1;
    while (from && !from->next.count(ch))
    {
        from->next[ch] = x;
        from = from->link;
    }
    if (!from)
        x->link = &root;
    else
    {
        SAMNode* const fNxt = from->next.at(ch);
        if (fNxt->len == from->len + 1)
            x->link = fNxt;
        else
        {
            SAMNode* cloned = cur++;
            cloned->link = fNxt->link;
            cloned->len = from->len + 1;
            for (auto i : fNxt->next)
                if (i.second->len)
                    cloned->next.insert(i);
            for (SAMNode* i = from; i; i = i->link)
            {
                const auto it = i->next.find(ch);
                if (it != i->next.end() && it->second == fNxt)
                    it->second = cloned;
                else
                    break;
            }
            fNxt->link = x->link = cloned;
        }
    }
}
void buildGSAM()
{
    queue<pair<SAMNode*, char>> q;
    for (auto i : root.next)
        q.emplace(&root, i.first);
    while (!q.empty())
    {
        const auto cur = q.front();
        q.pop();
        SAMNode* const ptr = cur.first->next[cur.second];
        insertChar(ptr, cur.second, cur.first);
        for (auto i : ptr->next)
            q.emplace(ptr, i.first);
    }
}
```