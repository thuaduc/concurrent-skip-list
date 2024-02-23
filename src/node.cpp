#include "node.hpp"

Node::Node(int key, int value, int level) : value(value), key(key) {
    forward.resize(level + 1);
}

int Node::getKey() { return this->key; }