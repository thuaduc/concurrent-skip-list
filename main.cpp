#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

#include "src/concurrentSkipList.cpp"



int main()
{
    ConcurrentSkipList<uint16_t, uint16_t, 4> csl{};
    // const int numThreads = 4;

    auto insert_and_delete = [&csl] () {
        for (uint16_t k = 0; k < 10; ++k) {
            csl.insertElement(k, k);
            csl.deleteElement(k);
        }
    };

    std::thread thread1(insert_and_delete);
    //std::thread thread2(insert_and_delete);
 
    thread1.join();
    //thread2.join();

    // csl.displayList();
    // assert(csl.size() == 0);
    // return 0;
}
