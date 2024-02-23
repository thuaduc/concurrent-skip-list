#pragma once
#include <iostream>
#include <memory>
#include <vector>

// Node struct for the ConcurrentSkipList
struct Node {
    Node(int key, int value, int level);
    ~Node() = default;
    int getKey();

    int value;
    int key;
    std::vector<std::shared_ptr<Node>> forward;
};