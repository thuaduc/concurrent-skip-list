#include "concurrentSkipList.hpp"

ConcurrentSkipList::ConcurrentSkipList(int maxLevel, float prop)
    : maxLevel(maxLevel), prob(prob) {
    this->header = createNode(INT_MIN, 0, maxLevel);
}

int ConcurrentSkipList::generateRandomLevel() {
    return rand() % this->maxLevel;
}

NodePtr ConcurrentSkipList::createNode(int key, int value, int level) {
    return std::make_shared<Node>(key, value, level);
}

void ConcurrentSkipList::insertElement(int key, int value) {
    // update is the forward array of the to be insert key
    auto update = std::vector<std::shared_ptr<Node>>();
    update.resize(this->maxLevel + 1);

    auto current = header;

    for (int i = this->currentLevel; i >= 0; --i) {
        while (current->forward[i] != nullptr &&
               current->forward[i].get()->getKey() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    // current to the fist element on lowest level
    current = current->forward[0];

    // case key already existed
    if (current != nullptr && current.get()->getKey() == key) {
        std::cerr << "key: " << key << " existed. Insert failed" << std::endl;
        return;
    }

    if (current == nullptr || current.get()->getKey() != key) {
        int randomLevel = generateRandomLevel();

        // If random level is greater thar skip list's current level, initialize
        // update value with pointer to header
        if (randomLevel > this->currentLevel) {
            for (int i = currentLevel + 1; i <= randomLevel; ++i) {
                update[i] = this->header;
            }
            this->currentLevel = randomLevel;
        }

        // create new node
        NodePtr insertedNode = createNode(key, value, randomLevel);

        // insert node
        for (int i = 0; i <= randomLevel; i++) {
            insertedNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = insertedNode;
        }
        std::cout << "Successfully inserted key:" << key << ", value:" << value
                  << std::endl;
        this->elementsCount++;
    }
}

void ConcurrentSkipList::deleteElement(int key) {}

void ConcurrentSkipList::searchElement(int key) {}

void ConcurrentSkipList::displayList() {
    std::cout << "Concurrent Skip-List" << std::endl;

    if (header->forward[0] == nullptr) {
        std::cout << "List is empty" << std::endl;
        return;
    }

    size_t len = static_cast<int>(this->elementsCount);

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
        std::cout << "Level " << i << ": Header -";
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
