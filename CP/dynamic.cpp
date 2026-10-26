#include <iostream>
#include <cstdlib>
#include <typeindex>

class dyn {
public:
    template<typename T>
    dyn(T val) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable.");
        data = realloc(data, sizeof(val));
        if (!data) throw std::bad_alloc();
        memcpy(data, &val, sizeof(val));
        t = typeid(val);
    }
    ~dyn() { free(data); data = nullptr; }

    template<typename T>
    void operator=(T val) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable.");
        data = realloc(data, sizeof(val));
        if (!data) throw std::bad_alloc();
        memcpy(data, &val, sizeof(val));
        t = typeid(T);
    }

    dyn(const dyn&) = delete;
    void operator=(dyn) = delete;

    template<typename T>
    T cast() const {
        if (!data) throw std::bad_cast(); 
        if (typeid(T) != t) throw std::bad_cast();
        return *static_cast<T*>(data);
    }

    template<typename T>
    void operator+=(T val) {
        T storedVal = cast<T>();
        storedVal += val;
        (*this) = storedVal;
    }
    template<typename T>
    void operator-=(T val) {
        T storedVal = cast<T>();
        storedVal -= val;
        (*this) = storedVal;
    }

private:
    void* data = nullptr;
    std::type_index t = typeid(nullptr);
};

int main() {
    dyn d = 5.14f;
    
    std::cout << d.cast<float>();

    return 0;
}