#include <iostream>
#include <vector>

template<typename T>
void add1(T& val) {
    val++;
}



int main() {
    int e = 5;
    add1(e);
    float num = 5.f;
    add1(num);

    return 0;
}