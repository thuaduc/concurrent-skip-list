#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

#include "src/concurrentSkipList.cpp"

int main()
{
    ConcurrentSkipList<uint16_t, uint16_t, 4> csl{};
    const int numThreads = 4;

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back([&csl]
                             {
            for (uint16_t k = 0; k < 10; ++k) {
                csl.insertElement(k, k);
                csl.deleteElement(k);
            } });
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    csl.displayList();
    assert(csl.size() == 0);
    return 0;
}
