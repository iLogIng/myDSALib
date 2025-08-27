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
    size_t size;
    size_t capacity;

    void reserve_capacity(size_t new_capacity) {
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
                new (&array[i++]) (item)
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
                new (&array[i]) (other[i]);
            }
        } else {
            array = nullptr;
        }
    }
    DynArray(DynArray&& other)
        : array(other.array), size(other.size), capacity(other.capacity)
    {
        other.array = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    DynArray& operator=(const DynArray& other) {
        if(this != &other) {
            DynArray temp(other);
            swap(temp);
        }
        return *this;
    }

    DynArray& operator=(DynArray&& other) {
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
    size_t getSize() noexcept { return size; }
    const size_t getSize() const noexcept { return size; }

    size_t getCapacity() noexcept { return capacity; }
    const size_t getCapacity() const noexcept { return capacity; }

    bool empty() { return size == 0; }

    Ty front() { return array[0]; }
    const Ty front() const { return array[0]; }
    Ty back() { return array[size - 1]; }
    const Ty back() const { return array[size - 1]; }

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

    void clear() {
        for(size_t i = 0; i < size; ++i) {
            array[i].~Ty();
        }
        size = 0;
    }

    void swap(DynArray& other) {
        swap(array, other.array);
        swap(size, other.size);
        swap(capacity, other.capacity);
    }

    void reserve(size_t new_capacity) {
        if(new_capacity > capacity) {
            reserve_capacity(new_capacity);
        }
    }

    void resize(size_t new_size, Ty value = Ty{}) {
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

    void shrink_to_fit() {
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

    void push_back(Ty& value) {
        if(size >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        new (&array[size]) Ty(value);
        ++size;
    }

    void push_back(Ty&& value) {
        if(size >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        new (&array[size]) Ty(value);
        ++size;
    }

    void emplace_back(Ty&& value) {
        if(size >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        new (&array[size]) Ty(std::move(value));
        ++size;
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if(size >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        new (&array[size]) Ty(std::forward<Args>(args)...);
        ++size;
    }

    void pop_back() {
        if(size > 0) {
            --size;
            array[size].~Ty();
        }
    }

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

}
}


