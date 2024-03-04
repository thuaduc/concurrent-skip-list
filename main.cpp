#include "src/concurrentSkipList.cpp"

int main() {
    ConcurrentSkipList<uint16_t, uint16_t, 4, 3> csl{};
    uint16_t length = 20;

    for (uint16_t i = length - 1; i >= 1; i--) {
        csl.insertElement(i, i);
    }

    for (uint16_t i = 1; i < length; i++) {
        if (csl.searchElement(i)) {
            std::cout << "True ";
        }
    }
    std::cout << std::endl;

    csl.displayList();

    for (uint16_t i = 1; i < length - 1; i++) {
        csl.deleteElement(i);
    }

    csl.displayList();
}