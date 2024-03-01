#pragma once
#include <atomic>
#include <climits>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>

#include "node.hpp"

template <typename T, unsigned blockSize, unsigned maxLevel>
struct ConcurrentSkipList {
   public:
    ConcurrentSkipList();
    int generateRandomLevel();
    std::shared_ptr<Node<T, blockSize>> createNode(T, int);
    bool searchElement(T);
    void insertElement(T);
    void deleteElement(T);
    void displayList();

   private:
    int currentLevel = 0;
    size_t elementsCount = 0;
    std::shared_ptr<Node<T, blockSize>> header;
};

template <typename T, unsigned blockSize, unsigned maxLevel>
ConcurrentSkipList<T, blockSize, maxLevel>::ConcurrentSkipList() {
    this->header = createNode(std::numeric_limits<T>::min(), maxLevel);
}

template <typename T, unsigned blockSize, unsigned maxLevel>
int ConcurrentSkipList<T, blockSize, maxLevel>::generateRandomLevel() {
    return rand() % maxLevel;
}

template <typename T, unsigned blockSize, unsigned maxLevel>
std::shared_ptr<Node<T, blockSize>>
ConcurrentSkipList<T, blockSize, maxLevel>::createNode(T key, int level) {
    return std::make_shared<Node<T, blockSize>>(key, level);
}

template <typename T, unsigned blockSize, unsigned maxLevel>
bool ConcurrentSkipList<T, blockSize, maxLevel>::searchElement(T key) {
    auto current = header;

    for (int i = this->currentLevel; i >= 0; --i) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->getKey() < key) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current != nullptr && current->getKey() == key) {
        return true;
    }

    return false;
}

template <typename T, unsigned blockSize, unsigned maxLevel>
void ConcurrentSkipList<T, blockSize, maxLevel>::insertElement(T key) {
    auto update = std::vector<std::shared_ptr<Node<T, blockSize>>>();
    update.resize(maxLevel + 1);

    auto current = header;

    for (int i = this->currentLevel; i >= 0; --i) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->getKey() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current != nullptr && current->getKey() == key) {
        std::cerr << "key: " << key << " existed. Insert failed" << std::endl;
        return;
    }

    if (current == nullptr || current->getKey() != key) {
        int randomLevel = generateRandomLevel();

        if (randomLevel > this->currentLevel) {
            for (int i = currentLevel + 1; i <= randomLevel; ++i) {
                update[i] = this->header;
            }
            this->currentLevel = randomLevel;
        }

        std::shared_ptr<Node<T, blockSize>> insertedNode =
            createNode(key, randomLevel);

        for (int i = 0; i <= randomLevel; i++) {
            insertedNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = insertedNode;
        }
        std::cout << "Successfully inserted key: " << key << std::endl;
        this->elementsCount++;
    }
}

template <typename T, unsigned blockSize, unsigned maxLevel>
void ConcurrentSkipList<T, blockSize, maxLevel>::deleteElement(T key) {
    auto update = std::vector<std::shared_ptr<Node<T, blockSize>>>();
    update.resize(maxLevel + 1);

    auto current = header;

    for (int i = this->currentLevel; i >= 0; --i) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->getKey() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current != nullptr && current->getKey() == key) {
        for (int i = 0; i <= this->currentLevel; i++) {
            if (update[i]->forward[i] != current) break;

            update[i]->forward[i] = current->forward[i];
        }
        elementsCount--;
    }
    return;
}

template <typename T, unsigned blockSize, unsigned maxLevel>
void ConcurrentSkipList<T, blockSize, maxLevel>::displayList() {
    std::cout << "Concurrent Skip-List" << std::endl;

    if (header->forward[0] == nullptr) {
        std::cout << "List is empty" << std::endl;
        return;
    }

    int len = static_cast<int>(this->elementsCount);

    std::vector<std::vector<std::string>> builder(
        len, std::vector<std::string>(this->currentLevel + 1));

    auto current = header->forward[0];

    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < this->currentLevel + 1; ++j) {
            if (j < static_cast<int>(current->forward.size())) {
                std::ostringstream oss;
                oss << std::setw(2) << std::setfill('0') << current->getKey();
                builder.at(i).at(j) = oss.str();
            } else {
                builder.at(i).at(j) = "--";
            }
        }
        current = current->forward.at(0);
    }

    for (int i = this->currentLevel; i >= 0; --i) {
        std::cout << "Level " << i << ": Header ";
        for (int j = 0; j < len; ++j) {
            if (builder.at(j).at(i) == "--") {
                std::cout << "------";
            } else {
                std::cout << "--->" << builder.at(j).at(i);
            }
        }
        std::cout << "--->Null" << std::endl;
    }
}
