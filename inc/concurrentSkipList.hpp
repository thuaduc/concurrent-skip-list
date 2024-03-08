#pragma once
#include <climits>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>
#include <shared_mutex>
#include <mutex>

#include "node.hpp"

template <typename T, typename K, unsigned maxLevel>
struct ConcurrentSkipList
{
public:
    ConcurrentSkipList();
    int generateRandomLevel();
    std::shared_ptr<Node<T, K>> createNode(T, K, int);

    bool searchElement(T);
    void insertElement(T, K);
    void deleteElement(T);
    void displayList();
    size_t size();

private:
    int currentLevel = 0;
    size_t elementsCount = 0;
    std::shared_mutex mutex;
    mutable std::shared_ptr<Node<T, K>> header;
    std::pair<std::shared_ptr<Node<T, K>>, std::vector<std::shared_ptr<Node<T, K>>>> searchHelper(T value);
};

template <typename T, typename K, unsigned maxLevel>
size_t ConcurrentSkipList<T, K, maxLevel>::size()
{
    return this->elementsCount;
}

template <typename T, typename K, unsigned maxLevel>
ConcurrentSkipList<T, K, maxLevel>::ConcurrentSkipList()
{
    this->header = createNode(std::numeric_limits<T>::min(), 0, maxLevel);
}

template <typename T, typename K, unsigned maxLevel>
int ConcurrentSkipList<T, K, maxLevel>::generateRandomLevel()
{
    return rand() % maxLevel;
}

template <typename T, typename K, unsigned maxLevel>
std::shared_ptr<Node<T, K>>
ConcurrentSkipList<T, K, maxLevel>::createNode(T key, K value,
                                               int level)
{
    return std::make_shared<Node<T, K>>(key, value, level);
}

template <typename T, typename K, unsigned maxLevel>
bool ConcurrentSkipList<T, K, maxLevel>::searchElement(T key)
{
    std::shared_lock lock(mutex);
    auto current = header;
    for (int i = this->currentLevel; i >= 0; --i)
    {
        while (current->forward[i] != nullptr &&
               current->forward[i]->getKey() < key)
        {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current != nullptr && current->getKey() == key)
    {
        return true;
    }

    return false;
}

template <typename T, typename K, unsigned maxLevel>
std::pair<std::shared_ptr<Node<T, K>>, std::vector<std::shared_ptr<Node<T, K>>>>
ConcurrentSkipList<T, K, maxLevel>::searchHelper(T key)
{
    auto current = header;
    auto update = std::vector<std::shared_ptr<Node<T, K>>>(maxLevel + 1);

    for (int i = this->currentLevel; i >= 0; --i)
    {
        while (current->forward[i] != nullptr &&
               current->forward[i]->getKey() < key)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    return std::make_pair(current->forward[0], update);
}

template <typename T, typename K, unsigned maxLevel>
void ConcurrentSkipList<T, K, maxLevel>::insertElement(T key,
                                                       K value)
{
    std::unique_lock lock(mutex);
    auto [current, update] = searchHelper(key);
    if (current != nullptr && current->getKey() == key)
    {
        std::cerr << "Key: " << key << " existed. Insert failed" << std::endl;
        return;
    }

    if (current == nullptr || current->getKey() != key)
    {
        int newLevel = generateRandomLevel();
        if (newLevel > this->currentLevel)
        {
            for (int i = currentLevel + 1; i <= newLevel; ++i)
            {
                update[i] = this->header;
            }
            this->currentLevel = newLevel;
        }

        std::shared_ptr<Node<T, K>> insertedNode = createNode(key, value, newLevel);

        for (int i = 0; i <= newLevel; i++)
        {
            insertedNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = insertedNode;
        }
        this->elementsCount++;
    }
}

template <typename T, typename K, unsigned maxLevel>
void ConcurrentSkipList<T, K, maxLevel>::deleteElement(T key)
{
    std::unique_lock lock(mutex);
    auto [current, update] = searchHelper(key);
    if (current != nullptr && current->getKey() == key)
    {
        for (int i = 0; i <= this->currentLevel; i++)
        {
            if (update[i]->forward[i] != current)
                break;

            update[i]->forward[i] = current->forward[i];
        }
        this->elementsCount--;
    }
    return;
}

template <typename T, typename K, unsigned maxLevel>
void ConcurrentSkipList<T, K, maxLevel>::displayList()
{
    std::cout << "Concurrent Skip-List" << std::endl;

    if (header->forward[0] == nullptr)
    {
        std::cout << "List is empty" << std::endl;
        return;
    }

    int len = static_cast<int>(this->elementsCount);

    std::vector<std::vector<std::string>> builder(
        len, std::vector<std::string>(this->currentLevel + 1));

    auto current = header->forward[0];
    for (int i = 0; i < len; ++i)
    {
        for (int j = 0; j < this->currentLevel + 1; ++j)
        {
            if (j < static_cast<int>(current->forward.size()))
            {
                std::ostringstream oss;
                oss << std::setw(2) << std::setfill('0') << current->getKey();
                builder.at(i).at(j) = oss.str();
            }
            else
            {
                builder.at(i).at(j) = "--";
            }
        }
        current = current->forward.at(0);
    }

    for (int i = this->currentLevel; i >= 0; --i)
    {
        std::cout << "Level " << i << ": Header ";
        for (int j = 0; j < len; ++j)
        {
            if (builder.at(j).at(i) == "--")
            {
                std::cout << "------";
            }
            else
            {
                std::cout << "--->" << builder.at(j).at(i);
            }
        }
        std::cout << "--->Null" << std::endl;
    }
}
