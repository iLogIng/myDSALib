#pragma once
#include <cstddef>
#include <utility>
#include <initializer_list>

namespace myDSALib
{
namespace Linear
{

template<typename Ty, size_t N>
class StaArray
{
private:
    Ty* array;

public:
    explicit StaArray(const Ty& value = Ty{})
        : array(static_cast<Ty*>(operator new[](N * sizeof(Ty))))
        {
            for(size_t i = 0; i < N; ++i)
                new (&array[i]) Ty(value);
        }
    explicit StaArray(std::initializer_list<Ty> init_list)
        : StaArray()
        {
            size_t i = 0;
            for(const auto& init : init_list) {
                if(i < N) new (&array[i++]) Ty(std::move(init));
            }
            for(; i < N; ++i)
                new (&array[i]) Ty();
        }

    StaArray(const StaArray&) = delete;
    StaArray& operator=(const StaArray&) = delete;

    StaArray(StaArray&& other) noexcept
        : array(other.array)
    {
        array = nullptr;
    }

    StaArray& operator=(StaArray&& other) noexcept {
        if(this != &other) {
            for(size_t i = 0; i < N; ++i)
                array[i].~Ty();
            operator delete[](array);
            array = other.array;
            other.array = nullptr;
        }
        return *this;
    }

    ~StaArray() {
        if(array) {
            for(size_t i = 0; i < N; ++i)
                array[i].~Ty();
            operator delete[](array);
        }
    }

public:
    Ty& operator[](size_t index) {
        return array[index];
    }
    const Ty& operator[](size_t index) const {
        return array[index];
    }

    Ty& at(size_t index) {
        if(index >= N)
            throw std::out_of_range("Index out of range");
        return array[index];
    }
    const Ty& at(size_t index) const {
        if(index >= N)
            throw std::out_of_range("Index out of range");
        return array[index];
    }

    // array's size
    constexpr size_t size() const noexcept { return N; }
    // array is empty
    constexpr size_t empty() const noexcept { return N == 0; }

    // the front elem
    constexpr Ty& front() noexcept { return array[0]; }
    // the front elem const.Ver.
    constexpr const Ty& front() const noexcept { return array[0]; }
    // the back elem
    Ty& back() noexcept { return array[N - 1]; }
    // the back elem const.Ver.
    const Ty& back() const noexcept { return array[N - 1]; }

    // swap
    void swap(StaArray<Ty, N> other) noexcept {
        swap(this->array, other.array);
    }

    // clear
    void clear() noexcept {
        for(size_t i = 0; i < N; ++i)
            array[i].~Ty();
    }

public:
    static_assert(N > 0,
        "Static Array's size N must be a Positive Number");
};

}
}