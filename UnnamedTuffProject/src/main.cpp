#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include <SDL3/SDL.h>

std::vector<std::string> choices = {"plus", "minus", "gange", "dividere", "slut"};
std::vector<std::string> choices_op = {"+", "-", "*", "/", "\nNej nej stop lige nu\n"};

std::pair<float, float> GetTwoNum() {
    std::string as;
    std::string bs;

    std::pair<float, float> result;

get_first_num:
    std::cout << "First number: ";
    std::getline(std::cin, as);
    try {
        result.first = std::stof(as);
    } catch(const std::exception& e) {
        std::cout << "Invalid first number\n";
        goto get_first_num;
    }

get_second_num:
    std::cout << "Second number: ";
    std::getline(std::cin, as);
    try {
        result.second = std::stof(as);
    } catch(const std::exception& e) {
        std::cout << "Invalid second number\n";
        goto get_second_num;
    }
    
    return result;
}

void Plus() {
    auto nums = GetTwoNum();

    float result = nums.first + nums.second;
    std::cout << nums.first << " + " << nums.second << " = " << result << "\n";
}

void Minus() {
    auto nums = GetTwoNum();

    float result = nums.first - nums.second;
    std::cout << nums.first << " - " << nums.second << " = " << result << "\n";
}

void Gang() {
    auto nums = GetTwoNum();

    float result = nums.first * nums.second;
    std::cout << nums.first << " * " << nums.second << " = " << result << "\n";
}

void Divider() {
    auto nums = GetTwoNum();
    if (nums.second == 0) {
        std::cout << "Dawg du kan ikke dividere med 0!🤣\n";
        return;
    }
    
    float result = nums.first / nums.second;
    std::cout << nums.first << " / " << nums.second << " = " << result << "\n";
}

void menu() {
menu_start:
    std::cout << "-------------------\n";
    std::cout << "       Menu        \n";
    std::cout << "-------------------\n";

    for (int i = 0; i < choices.size(); i++)
        std::cout << i + 1 << ": " << choices[i] << "\n";

    std::cout << "Choice: ";
    std::string input;
    std::getline(std::cin, input);
    

    try {
        int index = std::stoi(input) - 1;

        if (index + 1 == choices.size()) return;
        
        if (index < choices.size()) {
            std::cout << "\nDu valgte " << choices[index] << "!\n\n";
            std::cout << "Syntax for " << choices[index] << " er: " << "a " << choices_op[index] << " b\n\n";
            
            switch (index)
            {
            case 0:
                Plus();
                break;
            case 1:
                Minus();
                break;
            case 2:
                Gang();
                break;
            case 3:
                Divider();
                break;
            
            default:
                std::cout << "'" << index + 1 <<"' is an Invalid operation (wtf)\n";
                break;
            }
        } else
            std::cout << "'" << index + 1 <<"' is an Invalid operation\n";
        
    } catch (const std::exception& e) {
        std::cout << "Invalid input\n";
    }
    
    SDL_Delay(1000);

    goto menu_start;
}

int main() {
    std::cout << "-------------------\n";
    std::cout << "Velkommen til calc!\n"; //thats short for calculator
    std::cout << "-------------------\n";
    menu();
}