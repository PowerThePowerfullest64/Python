#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

constexpr uint64_t fib(int n) {
    uint64_t a = 0, b = 1;
    for (int i = 0; i < n; i++) {
        uint64_t next = a + b;
        a = b;
        b = next;
    }
    return a;
}

struct foo {
    void* data = nullptr;
};

int main() {
    uint64_t res = fib(120);
    printf("%zu", res);

    return 0;
}
