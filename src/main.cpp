#include "concurrentSkipList.hpp"

int main() {
    ConcurrentSkipList csl(6, 0.5);  // Max Level is 3, Probability is 0.5
    csl.insertElement(1, 1);
    csl.insertElement(2, 1);
    csl.insertElement(3, 1);
    csl.insertElement(8, 1);
    csl.insertElement(5, 1);
    csl.insertElement(7, 1);
    csl.insertElement(6, 1);
    csl.insertElement(4, 1);
    csl.insertElement(9, 1);

    csl.displayList();
}