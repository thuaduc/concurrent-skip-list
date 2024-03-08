#pragma once
#include <array>
#include <iostream>
#include <memory>
#include <vector>

template <typename T, typename K>
struct Node
{
    Node(T, K, int);
    ~Node() = default;
    T getKey() const;
    K getValue() const;

    std::vector<std::shared_ptr<Node>> forward;

private:
    T key;
    K value;
    int level;
};

template <typename T, typename K>
Node<T, K>::Node(T key, K value, int level) : key{key}, value{value}
{
    forward.resize(level + 1);
}

template <typename T, typename K>
T Node<T, K>::getKey() const
{
    return key;
}

template <typename T, typename K>
K Node<T, K>::getValue() const
{
    return value;
}
