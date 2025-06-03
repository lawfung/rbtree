#ifndef RBTREE_MULTISET_H
#define RBTREE_MULTISET_H

#include <iostream>

template<typename KeyType>
class RBTreeMultiset {
private:
    enum Color { RED, BLACK };
    struct Node {
        KeyType key;
        int size;
        int count;
        Color color;
        Node *left, *right, *parent;
        Node(const KeyType& k, Color c, Node* p)
            : key(k), size(1), count(1), color(c), left(nullptr), right(nullptr), parent(p) {}

        Node* next() {
            Node* cur = this;
            if (cur->right) {
                cur = cur->right;
                while (cur->left) cur = cur->left;
                return cur;
            }
            while (cur->parent && cur == cur->parent->right) cur = cur->parent;
            return cur->parent;
        }

        Node* prev() {
            Node* cur = this;
            if (cur->left) {
                cur = cur->left;
                while (cur->right) cur = cur->right;
                return cur;
            }
            while (cur->parent && cur == cur->parent->left) cur = cur->parent;
            return cur->parent;
        }
    };
    Node* m_root = nullptr;

    int node_size(Node* n) const { return n ? n->size : 0; }
    void update(Node* n) {
        if (n) n->size = n->count + node_size(n->left) + node_size(n->right);
    }

    void left_rotate(Node* &root, Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
        update(x);
        update(y);
    }

    void right_rotate(Node* &root, Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right) x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent) root = x;
        else if (y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        x->right = y;
        y->parent = x;
        update(y);
        update(x);
    }

    void insert_fixup(Node* &root, Node* z) {
        while (z->parent && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        left_rotate(root, z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    right_rotate(root, z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        right_rotate(root, z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    left_rotate(root, z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    Node* bst_insert_with_ptr(Node* &root, const KeyType& key, Node* parent, Node* &inserted) {
        if (!root) {
            inserted = new Node(key, RED, parent);
            return inserted;
        }
        if (key < root->key) {
            root->left = bst_insert_with_ptr(root->left, key, root, inserted);
        } else if (key > root->key) {
            root->right = bst_insert_with_ptr(root->right, key, root, inserted);
        } else {
            root->count++;
            inserted = nullptr;
        }
        update(root);
        return root;
    }

    Node* find_min(Node* n) const {
        while (n && n->left) n = n->left;
        return n;
    }

    Node* find_max(Node* n) const {
        while (n && n->right) n = n->right;
        return n;
    }

    void transplant(Node* &root, Node* u, Node* v) {
        if (!u->parent) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v) v->parent = u->parent;
    }

    void erase_impl(const KeyType& key) {
        Node* z = m_root;
        while (z && z->key != key) {
            if (key < z->key) z = z->left;
            else z = z->right;
        }
        if (!z) return;
        if (z->count > 1) {
            z->count--;
            update_upward(z);
            return;
        }
        Node* y = z;
        Node* x = nullptr;
        Color y_orig_color = y->color;
        if (!z->left) {
            x = z->right;
            transplant(m_root, z, z->right);
            update_upward((x ? x : y->parent));
        } else if (!z->right) {
            x = z->left;
            transplant(m_root, z, z->left);
            update_upward((x ? x : y->parent));
        } else {
            y = find_min(z->right);
            y_orig_color = y->color;
            x = y->right;
            Node* toUpdate = y;
            if (y->parent != z) {
                transplant(m_root, y, y->right);
                toUpdate = y->parent;
                y->right = z->right;
                if (y->right) y->right->parent = y;
            }
            transplant(m_root, z, y);
            y->left = z->left;
            if (y->left) y->left->parent = y;
            y->color = z->color;
            update_upward(toUpdate);
        }
        delete z;
        if (y_orig_color == BLACK) erase_fixup(m_root, x, (x ? x->parent : nullptr));
    }

    void erase_fixup(Node* &root, Node* x, Node* parent) {
        while ((x != root) && (!x || x->color == BLACK)) {
            if (parent) {
                if (x == parent->left) {
                    Node* w = parent->right;
                    if (w && w->color == RED) {
                        w->color = BLACK;
                        parent->color = RED;
                        left_rotate(root, parent);
                        w = parent->right;
                    }
                    if ((!w || !w->left || w->left->color == BLACK) &&
                        (!w || !w->right || w->right->color == BLACK)) {
                        if (w) w->color = RED;
                        x = parent;
                        parent = x ? x->parent : nullptr;
                    } else {
                        if (!w || !w->right || w->right->color == BLACK) {
                            if (w && w->left) w->left->color = BLACK;
                            if (w) w->color = RED;
                            if (w) right_rotate(root, w);
                            w = parent->right;
                        }
                        if (w) w->color = parent->color;
                        if (parent) parent->color = BLACK;
                        if (w && w->right) w->right->color = BLACK;
                        left_rotate(root, parent);
                        x = root;
                        parent = nullptr;
                    }
                } else {
                    Node* w = parent->left;
                    if (w && w->color == RED) {
                        w->color = BLACK;
                        parent->color = RED;
                        right_rotate(root, parent);
                        w = parent->left;
                    }
                    if ((!w || !w->left || w->left->color == BLACK) &&
                        (!w || !w->right || w->right->color == BLACK)) {
                        if (w) w->color = RED;
                        x = parent;
                        parent = x ? x->parent : nullptr;
                    } else {
                        if (!w || !w->left || w->left->color == BLACK) {
                            if (w && w->right) w->right->color = BLACK;
                            if (w) w->color = RED;
                            if (w) left_rotate(root, w);
                            w = parent->left;
                        }
                        if (w) w->color = parent->color;
                        if (parent) parent->color = BLACK;
                        if (w && w->left) w->left->color = BLACK;
                        right_rotate(root, parent);
                        x = root;
                        parent = nullptr;
                    }
                }
            } else {
                break;
            }
        }
        if (x) x->color = BLACK;
    }
    
    void update_upward(Node* n) {
        while (n) {
            update(n);
            n = n->parent;
        }
    }

    Node* find_by_order(Node* n, int k) const {
        if (!n) return nullptr;
        int lsz = node_size(n->left);
        if (k < lsz) return find_by_order(n->left, k);
        if (k < lsz + n->count) return n;
        return find_by_order(n->right, k - lsz - n->count);
    }

    int order_of_key(Node* n, const KeyType& key) const {
        if (!n) return 0;
        if (key < n->key) return order_of_key(n->left, key);
        if (key > n->key) return node_size(n->left) + n->count + order_of_key(n->right, key);
        return node_size(n->left);
    }

    void inorder(Node* n) const {
        if (!n) return;
        inorder(n->left);
        for (int i = 0; i < n->count; ++i)
            std::cout << n->key << " ";
        inorder(n->right);
    }

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

public:
    RBTreeMultiset() = default;
    ~RBTreeMultiset() { clear(); }

    int count(const KeyType& key) const {
        Node* n = m_root;
        while (n) {
            if (key < n->key) n = n->left;
            else if (key > n->key) n = n->right;
            else return n->count;
        }
        return 0;
    }

    int size() const {
        return node_size(m_root);
    }

    void clear() {
        destroy(m_root);
        m_root = nullptr;
    }

    void swap(RBTreeMultiset& other) {
        std::swap(m_root, other.m_root);
    }

    void insert(const KeyType& key) {
        Node* z = nullptr;
        m_root = bst_insert_with_ptr(m_root, key, nullptr, z);
        if (z) insert_fixup(m_root, z);
    }
    void erase(const KeyType& key) {
        erase_impl(key);
    }
    Node* find_by_order(int k) const { return find_by_order(m_root, k); }
    int order_of_key(const KeyType& key) const { return order_of_key(m_root, key); }
    void print_inorder() const { inorder(m_root); std::cout << std::endl; }

    Node* lower_bound(const KeyType& key) const {
        Node* n = m_root;
        Node* res = nullptr;
        while (n) {
            if (!(n->key < key)) {
                res = n;
                n = n->left;
            } else {
                n = n->right;
            }
        }
        return res;
    }

    Node* upper_bound(const KeyType& key) const {
        Node* n = m_root;
        Node* res = nullptr;
        while (n) {
            if (key < n->key) {
                res = n;
                n = n->left;
            } else {
                n = n->right;
            }
        }
        return res;
    }

    Node* begin() const { return find_min(m_root); }
    Node* last() const { return find_max(m_root); }
};

#endif // RBTREE_MULTISET_H
