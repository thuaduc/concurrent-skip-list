#pragma once
#include <array>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <vector>

template <typename T, typename K>
struct Node
{
    Node(T key, K value, int level);
    ~Node() = default;
    T getKey() const;
    K getValue() const;
    void setKey(T newKey);
    void setValue(K newValue);
    std::shared_ptr<Node> getForward(int i);
    void setForward(int i, std::shared_ptr<Node> newPtr);
    size_t getForwardSize();

private:
    T key;
    K value;
    int level;
    mutable std::shared_mutex mutex;
    std::vector<std::shared_ptr<Node>> forward;
};

template <typename T, typename K>
Node<T, K>::Node(T key, K value, int level) : key{key}, value{value}
{
    forward.resize(level + 1);
}

template <typename T, typename K>
std::shared_ptr<Node<T, K>> Node<T, K>::getForward(int i)
{
    std::shared_lock lock(mutex);
    return forward[i];
}

template <typename T, typename K>
void Node<T, K>::setForward(int i, std::shared_ptr<Node<T, K>> newPtr)
{
    std::unique_lock lock(mutex);
    forward[i] = newPtr;
}

template <typename T, typename K>
size_t Node<T, K>::getForwardSize()
{
    std::shared_lock lock(mutex);
    return forward.size();
}

template <typename T, typename K>
T Node<T, K>::getKey() const
{
    std::shared_lock lock(mutex);
    return key;
}

template <typename T, typename K>
K Node<T, K>::getValue() const
{
    std::shared_lock lock(mutex);
    return value;
}

template <typename T, typename K>
void Node<T, K>::setKey(T newKey)
{
    std::unique_lock lock(mutex);
    key = newKey;
}

template <typename T, typename K>
void Node<T, K>::setValue(K newValue)
{
    std::unique_lock lock(mutex);
    value = newValue;
}
