# /tests/CMakeLists.txt
# Add gTest int this Project

include_directories(
    ${CMAKE_SOURCE_DIR}/DSALib/include
)

add_executable(myDSALib_gTest

# Linear
    Linear/test_Array.cpp
    Linear/test_DeQue.cpp
    Linear/test_LinkedList.cpp
    Linear/test_Queue.cpp
    Linear/test_Stack.cpp

# Tree
    Tree/test_AVL.cpp
    Tree/test_BST.cpp
    Tree/test_BTree.cpp
    Tree/test_RBT.cpp

# Heap


# Graph
    Graph/test_AdjMatrix.cpp
    Graph/test_AdjList.cpp
    Graph/test_EdgeSet.cpp

)

target_link_libraries(myDSALib_gTest
    PRIVATE
        gtest_main
        gmock_main
        DSALib
)

target_include_directories(myDSALib_gTest
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_SOURCE_DIR}/includes
)

include(GoogleTest)
gtest_discover_tests(myDSALib_gTest)
