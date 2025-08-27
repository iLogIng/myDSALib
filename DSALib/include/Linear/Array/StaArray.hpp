#pragma once
#include <cstddef>
#include <utility>
#include <initializer_list>

namespace myDSALib
{
namespace Linear
{

template<typename Ty, size_t size>
class StaArray
{
private:
    Ty* array;

public:
    explicit StaArray()
        : array(new Ty[size]{}) { }
    explicit StaArray(Ty value = Ty{})
        : array(new Ty[size])
    {
        std::fill_n(array, size, value);
    }

    StaArray(const StaArray&) = delete;
    StaArray& operator=(const StaArray&) = delete;

    StaArray(StaArray&& other)
        : array(other.array)
    {
        array = nullptr;
    }

    StaArray& operator=(StaArray&& other) {
        if(this != &other) {
            delete[] array;
            array = other.array;
            other.array = nullptr;
        }
        return *this;
    }

    ~StaArray() {
        delete[] array;
    }

public:
    Ty& operator[](size_t index) {
        return array[index];
    }
    const Ty& operator[](size_t index) const {
        return array[index];
    }

    Ty& at(size_t index) {
        if(index >= size)
            throw std::out_of_range("Index out of range");
        return array[index];
    }
    Ty& at(size_t index) const {
        if(index >= size)
            throw std::out_of_range("Index out of range");
    }

    constexpr size_t getSize() noexcept { return size; }
    constexpr size_t empty() noexcept { return size == 0; }

};

}
}