#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main() {
    std::string niggas;
    std::getline(std::cin, niggas);
    std::cout << niggas << "\n";

    std::ofstream file("nigger.secret", std::ios::binary);
    file << niggas;
    file.close();

    std::ifstream notafile("nigger.secret");
    std::stringstream ss;
    ss << notafile.rdbuf();

    std::string contents = ss.str();
    std::cout << "Contents: " << contents;

    return 0;
}
