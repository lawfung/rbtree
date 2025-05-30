#include "RBTree_multiset.h"
#include <iostream>

int main() {
    RBTreeMultiset<int> ms;
    ms.insert(10);
    ms.insert(5);
    ms.insert(10);
    ms.insert(7);

    std::cout << "Inorder: ";
    ms.print_inorder(); // Output: 5 7 10 10

    std::cout << "Count of 10: " << ms.count(10) << "\n"; // Output: 2
    std::cout << "Size: " << ms.size() << "\n"; // Output: 4

    auto it = ms.lower_bound(8);
    if (it) std::cout << "Lower bound of 8: " << it->key << "\n"; // Output: 10

    ms.erase(10);
    std::cout << "After erase 10, count: " << ms.count(10) << "\n"; // Output: 1

    return 0;
}
