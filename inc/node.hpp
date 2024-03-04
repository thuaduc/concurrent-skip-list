#pragma once
#include <array>
#include <atomic>
#include <iostream>
#include <memory>
#include <vector>

template <typename T, typename K, unsigned blockSize>
struct Node {
    Node(T, K, int);
    ~Node() = default;
    T getKey() const;
    K getValue() const;
    std::pair<T, K> getKeyValue() const;
    void setKeyValue(T key, K value);

    bool isKeyExisted(T key) const;
    size_t elementsCount = 0;
    std::vector<std::shared_ptr<Node>> forward;

   private:
    std::array<std::atomic<T>, blockSize> keys;
    std::array<std::atomic<K>, blockSize> values;
    int level;
};

template <typename T, typename K, unsigned blockSize>
Node<T, K, blockSize>::Node(T key, K value, int level) {
    keys.at(0) = key;
    values.at(0) = value;
    forward.resize(level + 1);
}

template <typename T, typename K, unsigned blockSize>
T Node<T, K, blockSize>::getKey() const {
    return keys[0].load();
}

template <typename T, typename K, unsigned blockSize>
K Node<T, K, blockSize>::getValue() const {
    return values[0].load();
}

template <typename T, typename K, unsigned blockSize>
std::pair<T, K> Node<T, K, blockSize>::getKeyValue() const {
    return std::make_pair(keys[0].load(), values[0].load());
}

template <typename T, typename K, unsigned blockSize>
void Node<T, K, blockSize>::setKeyValue(T key, K value) {
    keys[0].store(key);
    values[0].store(value);
}

template <typename T, typename K, unsigned blockSize>
bool Node<T, K, blockSize>::isKeyExisted(T key) const {
    for (const auto& k : keys) {
        if (k.load() == key) return true;
    }
    return false;
}
