#include <iostream>
#include "DSALib/include/Linear/Array/StaArray.hpp"
#include "DSALib/include/Linear/Array/DynArray.hpp"
#include "DSALib/include/Linear/LinkedList/SigList.hpp"
#include "DSALib/include/Linear/LinkedList/DouList.hpp"
#include "DSALib/include/Linear/Stack.hpp"
#include "DSALib/include/Linear/Queue.hpp"
#include "DSALib/include/Linear/DeQue.hpp"

#include "DSALib/include/Tree/BST.hpp"

void DSALib_Linear_StaArray_test();
void DSALib_Linear_DynArray_test();
void DSALib_Linear_SigList_test();
void DSALib_Linear_DouList_test();
void DSALib_Linear_Stack_test();
void DSALib_Linear_Queue_test();
void DSALib_Linear_DeQue_test();

void DSALib_Tree_BST_test();

int main()
{
    std::cout << "Hello World!\n" << std::endl;

    // DSALib_Linear_StaArray_test();
    // DSALib_Linear_DynArray_test();
    // DSALib_Linear_SigList_test();
    // DSALib_Linear_DouList_test();
    // DSALib_Linear_Stack_test();
    // DSALib_Linear_Queue_test();
    // DSALib_Linear_DeQue_test();
    DSALib_Tree_BST_test();

    return 0;
}

void DSALib_Linear_StaArray_test() {

    using namespace myDSALib::Linear;

    std::cout << "StaArray Test\n" << std::endl;

    StaArray<int, 10> array(0);
    std::cout << array.size() << std::endl;
    std::cout << array[2] << std::endl;
    std::cout << array.at(2) << std::endl;

    array[2] = 1;
    std::cout << array[2] << std::endl;
    std::cout << array.empty() << std::endl;

}

void DSALib_Linear_DynArray_test() {

    using namespace myDSALib::Linear;

    std::cout << "DynArray Test\n" << std::endl;

    DynArray<int> array(10, 9);
    std::cout << array.getSize() << std::endl;
    std::cout << array.getCapacity() << std::endl;
    array[3] = 2;
    std::cout << array[3] << " " << array.at(3) << std::endl;
    array.resize(4);
    array.resize(10, 1);
    array.emplace_back(3);
    for(int i = 1; i < 10; ++i) {
        array.push_front(i);
    }
    for(size_t i = 0; i < array.getSize(); ++i) {
        std::cout << array.at(i) << " ";
    }
    std::cout << std::endl;

}

void DSALib_Linear_SigList_test() {

    using namespace myDSALib::Linear;

    std::cout << "SigList Test\n" << std::endl;

    SigList<int> list(1);
    list.push_front(2);
    list.push_front(3);
    list.push_back(4);
    list.pop_front();
    for(int i = 0; i < 3; ++i)
        list.pop_back();

    for(int i = 0; i < 5; ++i)
        list.push_back(i);
    list.reverse();
    list.emplace(list.end(), 9);
    list.emplace_front(8);
    list.emplace(list.end(), 22);

    for(auto l : list) {
        std::cout << l << std::endl;
    }

    return;
}

void DSALib_Linear_DouList_test() {

    using namespace myDSALib::Linear;

    std::cout << "DouList Test\n" << std::endl;

    DouList<int> list(1);
    list.push_front(13);
    list.push_back(32);
    list.reverse();
    for(auto l : list) {
        std::cout << l << std::endl;
    }
    // list.pop_front();
    // list.pop_back();
    // list.pop_back();

    return;
}

void DSALib_Linear_Stack_test() {

    using namespace myDSALib::Linear;

    std::cout << "Stack Test\n" << std::endl;

    std::cout << "Static Stack\n" << std::endl;

    Stack<int, 5> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    stack.push(5);
    std::cout << "if full ?\n" << stack.full() << std::endl << std::endl;
    for(size_t i = 0; i < stack.capacity(); ++i) {
        std::cout << stack.top() << std::endl;
        stack.pop();
    }
    std::cout << "if empty ?\n" << stack.empty() << std::endl << std::endl;


    std::cout << "Dynamic Stack\n" << std::endl;

    Stack<int> dyn;
    for(int i = 0; i < 3; ++i) {
        dyn.push(i);
    }
    std::cout << "size " << dyn.size() << std::endl;
    std::cout << "if empty ?\n" << dyn.empty() << std::endl << std::endl;
    size_t size = dyn.size();
    for(size_t i = 0; i < size; ++i) {
        std::cout << dyn.top() << std::endl;
        dyn.pop();
    }
    std::cout << "size " << dyn.size() << std::endl;
    std::cout << "if empty ?\n" << dyn.empty() << std::endl << std::endl;


    return;
}

void DSALib_Linear_Queue_test() {
    using namespace myDSALib::Linear;

    std::cout << "Queue Test\n" << std::endl;

    std::cout << "Dynamic Queue\n" << std::endl;
    Queue<int> dyn;
    for(int i = 0; i < 5; ++i) {
        dyn.push(i);
    }
    std::cout << "is queue empty ?\n" << dyn.empty() << std::endl;
    std::cout << "size " << dyn.size() << std::endl;
    size_t size = dyn.size();
    for(size_t i = 0; i < size; ++i) {
        std::cout << dyn.front() << std::endl;
        dyn.pop();
    }
    std::cout << "is queue empty ?\n" << dyn.empty() << std::endl;
    std::cout << "size " << dyn.size() << std::endl << std::endl;

    std::cout << "Static Queue\n" << std::endl;
    Queue<int, 5> sta;
    for(int i = 0; i < 5; ++i) {
        sta.push(i);
    }
    std::cout << "is queue empty ?\n" << sta.empty() << std::endl;
    std::cout << "size " << sta.size() << std::endl;
    size = sta.size();
    for(size_t i = 0; i < size; ++i) {
        std::cout << sta.front() << std::endl;
        sta.pop();
    }
    std::cout << "is queue empty ?\n" << sta.empty() << std::endl;
    std::cout << "size " << sta.size() << std::endl << std::endl;

}

void DSALib_Linear_DeQue_test() {
    using namespace myDSALib::Linear;

    std::cout << "DeQue Test\n" << std::endl;

    DeQue<int> deq;
    for(int i = 0; i < 5; ++i) {
        deq.push_back(i);
    }
    for(int i = 15; i < 20; ++i) {
        deq.push_front(i);
    }
    while(!deq.empty()) {
        std::cout << deq.front() << std::endl;
        deq.pop_front();
    }
    std::cout << "empty: " << deq.empty() << std::endl;

}

void DSALib_Tree_BST_test() {
    using namespace myDSALib::Tree;

    std::cout << "Binary Search Tree test\n" << std::endl;

    std::cout << "Tree Node test\n" << std::endl;

    BiTNode<int> node(99, makeBiNode<int>(9), makeNulBiNode<int>());
    std::cout << node.getData() << std::endl;
    std::cout << node.hasLeft() << std::endl;
    std::cout << node.hasRight() << std::endl;
    std::cout << node.hasTwice() << std::endl;

    std::cout << std::endl;
    std::cout << "Tree test\n" << std::endl;

    BST<int> tree(makeBiNode<int>(9));
    tree.insert(3);
    tree.insert(15);
    auto ptr = tree.find(3);
    if(ptr == nullptr) {
        std::cout << "nullptr\n";
    }
    else {
        std::cout << ptr->getData() << std::endl;
    }

    tree.insert(1);
    tree.insert(5);
    tree.insert(13);
    tree.insert(18);

    ptr = tree.findMin();
    if(ptr) {
        std::cout << "findMin() return " << ptr->getData() << std::endl;
    }
    ptr = tree.findMax();
    if(ptr) {
        std::cout << "findMax() return " << ptr->getData() << std::endl;
    }

    auto rm = tree.remove(9);
    std::cout << "deleted data: " << rm->getData() << std::endl;

    tree.clear();

    return;
}
