#include <iostream>
#include "DSALib/include/Linear/Array/StaArray.hpp"
#include "DSALib/include/Linear/Array/DynArray.hpp"
#include "DSALib/include/Linear/LinkedList/SigList.hpp"
//#include "DSALib/include/Linear/LinkedList/DouList.hpp"

void DSALib_Linear_StaArray_test();
void DSALib_Linear_DynArray_test();
void DSALib_Linear_SigList_test();
// void DSALib_Linear_DouList_test();

int main()
{
    std::cout << "Hello World!\n" << std::endl;

    // DSALib_Linear_StaArray_test();
    // DSALib_Linear_DynArray_test();
     DSALib_Linear_SigList_test();
    // DSALib_Linear_DouList_test();

    return 0;
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

void DSALib_Linear_SigList_test() {

    using namespace myDSALib::Linear;

    std::cout << "SigList Test\n" << std::endl;

}
#if 0
void DSALib_Linear_DouList_test() {

    using namespace myDSALib::Linear;

    std::cout << "DouList Test\n" << std::endl;

}
#endif

