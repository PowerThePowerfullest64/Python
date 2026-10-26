#include <iostream>
#include <vector>

template<typename T, typename... Args>
T Construct(Args... args) {
    return T(args...);
}

int main() {
    std::vector<int> v = Construct<std::vector<int>>(std::initializer_list<int>{5, 1, 15});
    for (int n : v)
        std::cout << n << " ";

    return 0;
}