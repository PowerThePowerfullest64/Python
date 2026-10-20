#include <stdlib.h>
#include <stdexcept>

template <typename T>
struct vector {
    static_assert(std::is_trivially_copyable_v<T>);

    T* data = nullptr;
    size_t count = 0;
    size_t capacity = 0;

    vector() = default;
    vector(const vector&) = delete;
    vector& operator=(const vector&) = delete;
    ~vector() { free(data); }

    void grow() {
        if (capacity > count)
            return;
        
        if (capacity > 0)
            capacity <<= 1;
        else capacity = 1;

        void* tmp = realloc(data, capacity * sizeof(T));
        if (tmp == nullptr)
            throw std::bad_alloc();

        data = static_cast<T*>(tmp);
    }

    void push(T val) {
        grow();
        data[count] = val;
        count++;
    }

    void pop() {
        if (count > 0) count--;
    }

    T& operator[](size_t index) {
        if (index >= count) throw std::out_of_range("Index out of range");
        return data[index];
    }
    const T& operator[](size_t index) const {
        if (index >= count) throw std::out_of_range("Index out of range");
        return data[index];
    }

    size_t size() const { return count; }
};