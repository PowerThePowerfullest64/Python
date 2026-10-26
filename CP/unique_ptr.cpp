#include <iostream>

struct Dog {
    void Bark() {
        std::cout << "Wuff!\n";
    }
};

template<typename T>
class UniquePtr {
public:
    UniquePtr() = delete;
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) { ptr = other.ptr; other.ptr = nullptr; }
    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        } return this;
    }

    UniquePtr(T* ptr) : ptr(ptr) {}
    ~UniquePtr() { delete(ptr); }

    T* get() { return ptr; }
    const T* get() const { return ptr; }

    T* operator->() {
        return ptr;
    }
    const T* operator->() const {
        return ptr;
    }
    T& operator*() {
        return *ptr;
    }
    const T& operator*() const {
        return *ptr;
    }

private:
    T* ptr = nullptr;
};

template<typename T, typename... Args>
UniquePtr<T> makeUnique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

int main() {
    UniquePtr ptr = makeUnique<int>(16);
    std::cout << *ptr << "\n";

    return 0;
}