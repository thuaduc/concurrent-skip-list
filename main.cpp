#include "inc/concurrentSkipList.hpp"

int main() {
    ConcurrentSkipList<uint16_t, 4, 3> csl{};
    uint16_t length = 3;

    for (uint16_t i = 1; i < length; i++) {
        csl.insertElement(i);
    }

    for (uint16_t i = 1; i < length; i++) {
        if (csl.searchElement(i)) {
            std::cout << "True ";
        }
    }
    std::cout << std::endl;

    csl.displayList();

    for (uint16_t i = 1; i < length; i++) {
        csl.deleteElement(i);
    }

    csl.displayList();
}