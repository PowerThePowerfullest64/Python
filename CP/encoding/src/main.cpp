#include <iostream>

#include "rl_encoding.hpp"

int main() {
    //rlEncodeFile("input.txt", "encoded.dat");
    rlDecodeFile("encoded.dat", "decoded.txt");

    return 0;
}
