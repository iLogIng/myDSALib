#include <gtest/gtest.h>
#include "../../DSALib/include/Linear/DeQue.hpp"

using namespace myDSALib::Linear;

TEST(DeQueTestSuite, PushTest) {
    DeQue<int> deq;
    deq.push_front(1);
    deq.push_front(2);
    deq.push_front(3);
    EXPECT_EQ(deq.size(), 3);
    deq.push_back(5);
    deq.push_back(13);
    deq.push_back(20);
    EXPECT_EQ(deq.front(), 3);
}

TEST(DeQueTestSuite, PopTest) {
    DeQue<int> deq;
    deq.clear();
    deq.push_front(3);
    deq.push_back(5);
    deq.pop_back();
    EXPECT_EQ(deq.back(), 3);
}
