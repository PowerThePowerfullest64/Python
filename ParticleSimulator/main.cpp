#include <iostream>
#include <thread>
#include <array>

void Count() {
    for (int i = 0; i < 1000; ++i)
        std::cout << i << " ";
    std::cout << "\n";
}

int main() {
    std::cout << "Hello, all niggers!\n";

    std::array<std::thread, 100> threads;
    for (int i = 0; i < threads.size(); i++) {
        threads[i] = std::thread(Count);
    }

    for (auto& thread : threads)
        thread.join();
}