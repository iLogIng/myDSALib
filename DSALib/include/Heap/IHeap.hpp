#pragma once

#include <memory>
#include "../Linear/Array/DynArray.hpp"

namespace myDSALib
{
namespace Heap
{

template<typename Ty>
class IHeap
{
public:
    virtual ~IHeap() = default;

public:
    // push the elem into the heap
    virtual void push(const Ty&) = 0;
    virtual void push(Ty&&) = 0;
    // heap's top elem
    virtual const Ty& top() const = 0;
    // pop the top elem of heap
    virtual Ty pop() = 0;

    // if heap empty?
    virtual bool empty() const = 0;
    // the size of heap
    virtual size_t size() const = 0;
    // clear the heap
    virtual void clear() = 0;

    // turn the array to heap
    virtual void heapify(const myDSALib::Linear::DynArray<Ty>&) = 0;
    // merge the heap
    virtual void merge(IHeap<Ty>&) = 0;

    // turn this heap to array
    virtual myDSALib::Linear::DynArray<Ty> to_array() const = 0;
    // check the attribute of this heap
    virtual bool validate() const = 0;

};

} // namespace Heap
} // namespace myDSALib
