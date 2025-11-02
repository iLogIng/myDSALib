
#include "../../DSALib/include/Linear/Array/DynArray.hpp"
#include "../../DSALib/include/Linear/Array/StaArray.hpp"
#include <gtest/gtest.h>

using namespace myDSALib::Linear;

#if 0
TEST_P(DynArrayTestSuite, InsertTest) {

    int pos = 0;
    int value = 1;
    myDSALib::Linear::DynArray<int> expected();

    myDSALib::Linear::DynArray<int> arr({1, 2, 3});

    arr.push_front(1);
    EXPECT_EQ(arr[1], expected);

}
#endif

#if 1

DynArray<int> dyn({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

TEST(DynArrayTestSuite, IndexTest) {
    EXPECT_EQ(dyn[0], 0);
    EXPECT_EQ(dyn[3], 3);
    EXPECT_EQ(dyn[9], 9);
    EXPECT_EQ(dyn.at(5), 5);
    EXPECT_EQ(dyn.at(4), 4);
    EXPECT_EQ(dyn.at(2), 2);
}

// ===============================================================

StaArray<int, 10> sta({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

TEST(StaArrayTestSuite, IndexTest) {
    EXPECT_EQ(sta[0], 0);
    EXPECT_EQ(sta[3], 3);
    EXPECT_EQ(sta[9], 9);
    EXPECT_EQ(sta.at(5), 5);
    EXPECT_EQ(sta.at(4), 4);
    EXPECT_EQ(sta.at(2), 2);
}

#endif
