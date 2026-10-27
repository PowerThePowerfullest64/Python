#include <iostream>
#include <string>
#include <format>
#include <fstream>


int main() {
    int generation = 0;

    constexpr auto program = R"{1}(#include <iostream>
#include <string>
#include <format>
#include <fstream>

int main() {{
    int generation = {3};

    constexpr auto program = R"{2}({0}){2}";

    std::cout << "Hello, World! Running generation " << generation << std::endl;

    std::string final_program = std::format(program, program, "Outer", "Inner", generation + 1);

    if (generation >= 100)
        return 0;

    std::string filename = "copy" + std::to_string(generation+1);
    std::ofstream srcfile((filename + ".cpp").c_str());
    srcfile << final_program;
    srcfile.close();

    std::system(("g++ -std=c++20 " + filename + ".cpp -o " + filename + ".exe").c_str());
    std::system((".\\" + filename + ".exe").c_str());

    return 0;
}}){1}";
    
    std::cout << "Hello, World! Running generation " << generation << std::endl;

    std::string final_program = std::format(program, program, "Outer", "Inner", generation + 1);

    if (generation >= 100)
        return 0;

    std::string filename = "copy" + std::to_string(generation+1);
    std::ofstream srcfile((filename + ".cpp").c_str());
    srcfile << final_program;
    srcfile.close();

    std::system(("g++ -std=c++20 " + filename + ".cpp -o " + filename + ".exe").c_str());
    std::system((".\\" + filename + ".exe").c_str());

    return 0;
}