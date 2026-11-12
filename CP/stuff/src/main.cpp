#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "commands.hpp"

namespace fs = std::filesystem;

std::vector<std::string> SplitByWhitespace(const std::string& s) {
    std::vector<std::string> res;

    bool inWord = false;
    for (char c : s) {
        if (c != ' ') {
            if (!inWord) {
                res.emplace_back(1, c);
                inWord = true;
            } else {
                res.back() += c;
            }
        } else {
            inWord = false;
        }
    }

    return res;
}

void ProcessCommand(const std::string& input, bool& running, std::vector<std::string>& taskList) {
    auto processedInput = SplitByWhitespace(input);

    if (processedInput.size() == 0) {
        return;
    }

    std::string command = processedInput[0];
    
    // remove command from args
    std::vector<std::string> args;

    if (command.size() > 0)
        for (int i = 1; i < processedInput.size(); ++i)
            args.push_back(processedInput[i]);
    
    bool foundCommand = false;

    if (command == "exit" || command == "quit") {
        running = false;
        foundCommand = true;
    }

    for (Command& c : commands) {
        if (c.IsCommand(command)) {
            c.Execute(args, taskList);
            foundCommand = true;
            break;
        }
    }

    if (!foundCommand)
        std::cout << "Command '" << command << "' not recognized.\n";
}

int main() {
    std::cout << "----- COMMAND LINE INTERFACE -----\n";

    bool running = true;
    std::vector<std::string> taskList;

    if (fs::exists("tasks.txt")) {
        std::ifstream file("tasks.txt");
        
        std::string line;
        while (std::getline(file, line)) {
            taskList.push_back(line);
        }

        if (!file.eof()) {
            std::cerr << "Something about reading tasks from tasks.txt failed...\n";
        }

        file.close();
    }

    while (running) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        ProcessCommand(input, running, taskList);
    }

    std::ofstream file("tasks.txt");
    for (const std::string& task : taskList)
        file << task << "\n";
    file.close();

    std::cout << "----- END OF PROGRAM -----\n";
}