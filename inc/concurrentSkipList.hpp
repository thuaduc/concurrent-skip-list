#pragma once
#include <atomic>
#include <climits>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#include "node.hpp"

using NodePtr = std::shared_ptr<Node>;

struct ConcurrentSkipList {
   public:
    ConcurrentSkipList(int, float);
    int generateRandomLevel();
    NodePtr createNode(int, int, int);
    void insertElement(int, int);
    void deleteElement(int);
    void searchElement(int);
    void displayList();

    int maxLevel;
    float prob;
    int currentLevel = 0;
    size_t elementsCount = 0;
    NodePtr header;
};
