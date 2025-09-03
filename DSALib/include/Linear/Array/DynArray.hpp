#pragma once
#include <cstddef>
#include <utility>
#include <initializer_list>

namespace myDSALib
{
namespace Linear
{

template<typename Ty>
class DynArray
{
private:
    Ty* array;
    size_t size = 0;
    size_t capacity;

    void reserve_capacity(size_t new_capacity);

public:
    explicit DynArray()
        : array(nullptr), size(0), capacity(0) { }
    explicit DynArray(size_t s, Ty value = Ty{})
        : size(s), capacity(s)
    {
        if(size > 0) {
            array = static_cast<Ty*>(operator new[](s * sizeof(Ty)));
            for(size_t i = 0; i < s; ++i)
                new (&array[i]) Ty(value);
        } else {
            array = nullptr;
        }
    }
    explicit DynArray(std::initializer_list<Ty> init_list)
        : size(init_list.size()), capacity(init_list.size())
    {
        if(size > 0) {
            array = (Ty*)static_cast(operator new[](size * sizeof(Ty)));
            size_t i = 0;
            for(auto& item : init_list) {
                new (&array[i++]) Ty(item);
            }
        } else {
            array = nullptr;
        }
    }

    DynArray(const DynArray& other)
        : size(other.size), capacity(other.size)
    {
        if(size >= 0) {
            array = (Ty*)(operator new[](other.size * sizeof(Ty)));
            for(size_t i = 0; i < size; ++i) {
                new (&array[i]) Ty(other[i]);
            }
        } else {
            array = nullptr;
        }
    }
    DynArray(DynArray&& other) noexcept
        : array(other.array), size(other.size), capacity(other.capacity)
    {
        other.array = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    DynArray& operator=(const DynArray& other) noexcept {
        if(this != &other) {
            DynArray temp(other);
            swap(temp);
        }
        return *this;
    }

    DynArray& operator=(DynArray&& other) noexcept {
        if(this != &other) {
            clear();
            operator delete[](array);

            array = other.array;
            size = other.size;
            capacity = other.capacity;

            other.array = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    ~DynArray() {
        clear();
        operator delete[](array);
    }

public:
    // array's size
    size_t getSize() noexcept { return size; }
    // array's size const.Ver.
    const size_t getSize() const noexcept { return size; }

    // array's capacity
    size_t getCapacity() noexcept { return capacity; }
    // array's capacity const.Ver.
    const size_t getCapacity() const noexcept { return capacity; }

    // array is empty
    bool empty() const noexcept { return size == 0; }

    // the front elem
    Ty& front() noexcept { return array[0]; }
    // the front elem const.Ver.
    const Ty& front() const noexcept { return array[0]; }
    // the back elem
    Ty& back() noexcept { return array[size - 1]; }
    // the back elem const.Ver.
    const Ty& back() const noexcept { return array[size - 1]; }

    Ty& operator[](size_t index) { return array[index]; }
    const Ty& operator[](size_t index) const { return array[index]; }

    Ty& at(size_t index) {
        if(index >= size)
            throw std::out_of_range("Index out of range");
        return array[index];
    }
    const Ty& at(size_t index) const {
        if(index >= size)
            throw std::out_of_range("Index out of range");
        return array[index];
    }

public:
    // clear the array
    void clear() noexcept {
        for(size_t i = 0; i < size; ++i) {
            array[i].~Ty();
        }
        size = 0;
    }

    // swap the array
    void swap(DynArray& other) noexcept {
        swap(array, other.array);
        swap(size, other.size);
        swap(capacity, other.capacity);
    }

    // set new capacity of array
    void reserve(size_t new_capacity) {
        if(new_capacity > capacity) {
            reserve_capacity(new_capacity);
        }
    }

    // set new size of array
    void resize(size_t new_size, Ty value = Ty{});

    // set capacity to fit the size
    void shrink_to_fit() noexcept;

    // back insert
    void push_back(const Ty& value);
    // move back insert
    void push_back(Ty&& value);
    // back emplace
    void emplace_back(Ty&& value);
    // back emplace
    template<typename... Args>
    void emplace_back(Args&&... args);

    // front insert
    void push_front(const Ty& value);
    // move front insert
    void push_front(Ty&& value);
    // front emplace
    void emplace_front(Ty&& value);
    // front emplace
    template<typename... Args>
    void emplace_front(Args&&... args);

    // remove back elem
    void pop_back() noexcept;

    // remove front elem
    void pop_front() noexcept;

};

template<typename Ty>
void swap(DynArray<Ty>& left, DynArray<Ty>& right) {
    left.swap(right);
}

template<typename Ty>
bool operator==(const DynArray<Ty>& left, const DynArray<Ty>& right) {
    if(left.getSize() != right.getSize())
        return false;
    for(size_t i = 0; i < left.getSize(); ++i) {
        if(left[i] != right[i])
            return false;
    }
    return true;
}

template<typename Ty>
bool operator!=(const DynArray<Ty>& left, const DynArray<Ty>& right) {
    return !(left == right);
}

// ================================================

template<typename Ty>
void DynArray<Ty>::reserve_capacity(size_t new_capacity) {
    if(new_capacity <= capacity)
        return;

    Ty* new_array = (Ty*)(operator new[](new_capacity * sizeof(Ty)));

    for(size_t i = 0; i < size; ++i) {
        try {
            new (&new_array[i]) Ty(std::move(array[i]));
        } catch(...) {
            for(size_t s = 0; s < size; ++s) {
                new_array[i].~Ty();
            }
            operator delete[](new_array);
            throw;
        }
        new_array[i].~Ty();
    }

    operator delete[](array);
    array = new_array;
    capacity = new_capacity;
}

template<typename Ty>
void DynArray<Ty>::resize(size_t new_size, Ty value) {
    if(new_size > size) {
        if(new_size > capacity)
            reserve(new_size);

        for(size_t i = size; i < new_size; ++i) {
            new (&array[i]) Ty(value);
        }
    } else if(new_size < size) {
        for(size_t i = new_size; i < size; ++i) {
            array[i].~Ty();
        }
    }
    size = new_size;
}

template<typename Ty>
void DynArray<Ty>::shrink_to_fit() noexcept {
    if(capacity > size) {
        if(size == 0) {
            operator delete[](array);
            capacity = 0;
            array = nullptr;
        } else {
            reserve_capacity(size);
        }
    }
}

template<typename Ty>
void DynArray<Ty>::push_back(const Ty& value) {
    if(size >= capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }
    new (&array[size]) Ty(value);
    ++size;
}

template<typename Ty>
void DynArray<Ty>::push_back(Ty&& value) {
    if(size >= capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }
    new (&array[size]) Ty(std::forward<Ty>(value));
    ++size;
}

template<typename Ty>
void DynArray<Ty>::emplace_back(Ty&& value) {
    if(size >= capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }
    new (&array[size]) Ty(std::move(value));
    ++size;
}

template<typename Ty>
template<typename... Args>
void DynArray<Ty>::emplace_back(Args&&... args) {
    if(size >= capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }
    new (&array[size]) Ty(std::forward<Args>(args)...);
    ++size;
}

template<typename Ty>
void DynArray<Ty>::push_front(const Ty& value) {
    if(size >= capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }
    for(size_t i = size; i > 0; --i) {
        new (&array[i]) Ty(std::move(array[i - 1]));
        array[i - 1].~Ty();
    }
    new (&array[0]) Ty(std::move(value));
    ++size;
}

template<typename Ty>
void DynArray<Ty>::push_front(Ty&& value) {
    if( size >= capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }
    for(size_t i = size; i > 0; --i) {
        new (&array[i]) Ty(std::move(array[i - 1]));
        array[i - 1].~Ty();
    }
    new (&array[0]) Ty(std::forward<Ty>(value));
    ++size;
}

template<typename Ty>
void DynArray<Ty>::emplace_front(Ty&& value) {
    if( size >= capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }
    for(size_t i = size; i > 0; --i) {
        new (&array[i]) Ty(std::move(array[i - 1]));
        array[i - 1].~Ty();
    }
    new (&array[0]) Ty(std::forward<Ty>(value));
    ++size;
}

template<typename Ty>
template<typename... Args>
void DynArray<Ty>::emplace_front(Args&&... args) {
    if( size >= capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }
    for(size_t i = size; i > 0; --i) {
        new (&array[i]) Ty(std::move(array[i - 1]));
        array[i - 1].~Ty();
    }
    new (&array[0]) Ty(std::forward<Args>(args)...);
    ++size;
}

template<typename Ty>
void DynArray<Ty>::pop_back() noexcept {
    if(size > 0) {
        --size;
        array[size].~Ty();
    }
}

template<typename Ty>
void DynArray<Ty>::pop_front() noexcept {
    if(size > 0) {
        --size;
        for(size_t i = 0; i < size; ++i) {
            array[i].~Ty();
            new (&array[i]) Ty(std::move(array[i + 1]));
        }
    }
}

}
}


