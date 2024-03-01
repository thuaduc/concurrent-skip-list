#pragma once
#include <array>
#include <iostream>
#include <memory>
#include <vector>

template <typename T, unsigned blockSize>
struct Node {
    Node(T key, int level);
    ~Node() = default;
    T getKey() const;
    std::vector<std::shared_ptr<Node>> forward;

   private:
    std::array<T, blockSize> keys;
    int level;
};

template <typename T, unsigned blockSize>
Node<T, blockSize>::Node(T key, int level) {
    keys.at(0) = key;
    forward.resize(level + 1);
}

template <typename T, unsigned blockSize>
T Node<T, blockSize>::getKey() const {
    return keys[0];
}
