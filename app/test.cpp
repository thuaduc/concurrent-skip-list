#include <gtest/gtest.h>

#include "../src/concurrentSkipList.cpp"

TEST(ConcurrentSkipList, Case Insert) {
    ConcurrentSkipList<uint16_t, uint16_t, 4> csl{};
    uint16_t length = 10;

    for (uint16_t i = length; i >= 1; i--) {
        csl.insertElement(i, i);
    }

    EXPECT_EQ(csl.elementsCount, length);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}