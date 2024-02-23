#include <iostream>
#include <memory>
#include <vector>

struct Node {
    int a, b;
    Node(int a, int b) : a(a), b(b) {}
};

int main() {
    std::vector<std::shared_ptr<Node>> v;
    v.resize(5);

    for (auto& a : v) {
        std::cout << a << " ";
        std::cout << static_cast<bool>(a == nullptr) << std::endl;
    }
}