#include <iostream>
#include "DSALib/include/Linear/LinkedList/sNode.hpp"
#include "DSALib/include/Linear/LinkedList/dNode.hpp"
#include "DSALib/include/Linear/Array/StaArray.hpp"
#include "DSALib/include/Linear/Array/DynArray.hpp"
//#include "DSALib/include/Linear/LinkedList/SigList.hpp"
//#include "DSALib/include/Linear/LinkedList/DouList.hpp"

void DSALib_Linear_sNode_test();
void DSALib_Linear_dNode_test();
void DSALib_Linear_StaArray_test();
void DSALib_Linear_DynArray_test();
//void DSALib_Linear_SigList_test();
// void DSALib_Linear_DouList_test();

int main()
{
    std::cout << "Hello World!\n" << std::endl;

    // DSALib_Linear_sNode_test();
     DSALib_Linear_dNode_test();
    // DSALib_Linear_StaArray_test();
    // DSALib_Linear_DynArray_test();
    // DSALib_Linear_SigList_test();
    // DSALib_Linear_DouList_test();

    return 0;
}


void DSALib_Linear_sNode_test() {

    using namespace myDSALib::Linear;

    std::cout << "sNode Test.\n" << std::endl;

    auto root = makeSigNode<int>(1, nullptr);
    auto tail = makeSigNode<int>(99, nullptr);
    root->setNext(tail);
    sNode<int>* temp;

    auto midl = makeSigNode<int>(55);
    root->insertNext(midl);
    temp = root.get();
    while(temp != nullptr) {
        std::cout << temp->getData() << std::endl;
        temp = temp->Next();
    }

    std::cout << root->hasNext() << std::endl;
    if(root->hasNext())
        root->removeNext();
    std::cout << root->hasNext() << std::endl;

    temp = root.get();
    while(temp != nullptr) {
        std::cout << temp->getData() << std::endl;
        temp = temp->Next();
    }

    std::cout << root->hasNext() << std::endl;
    if(root->hasNext())
        root->removeNext();
    std::cout << root->hasNext() << std::endl;

    auto node = makeSigNode<int>(100, nullptr);
    std::cout << node->getData() << std::endl;

    return;
}

void DSALib_Linear_dNode_test() {

    using namespace myDSALib::Linear;

    std::cout << "dNode Test\n" << std::endl;

    auto root = makeDouNode<int>(1);
    auto tail = makeDouNode<int>(99);
    root->insertNext(tail);
    std::cout << "root: " << root->hasPrev() << " " << root->hasNext() << std::endl;
    std::cout << "tail: " << tail->hasPrev() << " " << tail->hasNext() << std::endl;

    dNode<int>* temp;
    temp = root.get();
    while(temp != nullptr) {
        std::cout << temp->getData() << std::endl;
        temp = temp->Next();
    }
    if(temp == nullptr)
        std::cout << "temp: nullptr\n";
    temp = tail.get();
    std::cout << temp->hasNext() << " " << temp->hasPrev() << std::endl;

    while(temp != nullptr) {
        std::cout << temp->getData() << std::endl;
        temp = temp->Prev();
    }
    if(temp == nullptr)
        std::cout << "temp: nullptr\n";
    temp = root.get();
    std::cout << temp->hasNext() << " " << temp->hasPrev() << std::endl;

    auto node = makeDouNode<int>(1000, nullptr, nullptr);
    std::cout << node->getData() << std::endl;

}

void DSALib_Linear_StaArray_test() {

    using namespace myDSALib::Linear;

    std::cout << "StaArray Test\n" << std::endl;

    StaArray<int, 10> array(0);
    std::cout << array.getSize() << std::endl;
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
    for(size_t i = 0; i < array.getSize(); ++i) {
        std::cout << array.at(i) << " ";
    }
    std::cout << std::endl;

}
#if 0
void DSALib_Linear_SigList_test() {

    using namespace myDSALib::Linear;

    std::cout << "SigList Test\n" << std::endl;

    SigList<int> list(makeSigNode<int>(1));
    list.headInsert(makeSigNode<int>(9));
    auto node = list.getRoot();
    while(node) {
        std::cout << node->getData() << std::endl;
        node = node->Next();
    }
    node = list.findNode(1);
    std::cout << node->getData() << std::endl;
    node = list.getRoot();
    std::cout << node->getData() << std::endl;
    std::cout << list.hasCircle() << std::endl;

    std::cout << list.reverse() << std::endl;
    node = list.getRoot();
    while(node) {
        std::cout << node->getData() << std::endl;
        node = node->Next();
    }

    SigList<int> test(std::move(list));
    test.headInsert(makeSigNode<int>(19));
    node = test.getRoot();
    while(node) {
        std::cout << node->getData() << std::endl;
        node = node->Next();
    }

}

void DSALib_Linear_DouList_test() {

    using namespace myDSALib::Linear;

    std::cout << "DouList Test\n" << std::endl;

    DouList<int> list(makeDouNode<int>(1));

    list.headInsert(makeDouNode<int>(9));
    auto node = list.getRoot();
    while(node) {
        std::cout << node->getData() << std::endl;
        node = node->Next();
    }
    node = list.findNode(1);
    std::cout << node->getData() << std::endl;
    node = list.getRoot();

    DouList<int> test(std::move(list));
    list.tailInsert(makeDouNode<int>(996));
    node = list.getRoot();
    std::cout << node->getData() << std::endl;

}
#endif

