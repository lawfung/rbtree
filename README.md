# RBTreeMultiset

A C++ Red-Black Tree based multiset implementation supporting duplicate elements, order-statistics, and efficient queries.

## Public Member Functions

- `void insert(const KeyType& key)`  
  Insert an element into the multiset.

- `void erase(const KeyType& key)`  
  Remove one occurrence of the element from the multiset (if it exists).

- `int count(const KeyType& key) const`  
  Return the number of occurrences of the given key.

- `int size() const`  
  Return the total number of elements (including duplicates).

- `void clear()`  
  Remove all elements from the multiset.

- `void swap(RBTreeMultiset& other)`  
  Swap contents with another multiset.

- `void print_inorder() const`  
  Print all elements in sorted order.

- `Node* find_by_order(int k) const`  
  Return a pointer to the k-th (0-based) smallest element, or nullptr if out of range.

- `int order_of_key(const KeyType& key) const`  
  Return the number of elements strictly less than the given key.

- `Node* lower_bound(const KeyType& key) const`  
  Return a pointer to the first element not less than the given key, or nullptr if not found.

- `Node* upper_bound(const KeyType& key) const`  
  Return a pointer to the first element greater than the given key, or nullptr if not found.

- `Node* begin() const`  
  Return a pointer to the smallest element, or nullptr if the multiset is empty.

- `Node* last() const`  
  Return a pointer to the largest element, or nullptr if the multiset is empty.
