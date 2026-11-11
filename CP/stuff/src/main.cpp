#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

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

    if (command == "exit" || command == "quit") {
        running = false;
        return;
    } else if (command == "add" && processedInput.size() == 2) {
        taskList.push_back(processedInput[1]);
        return;
    } else if (command == "list") {
        if (taskList.size() == 0) {
            std::cout << "No tasks to list.\n";
            return;
        }

        for (int i = 0; i < taskList.size(); ++i)
            std::cout << "[" << i+1 << "] " << taskList[i] << "\n";
        return;
    } else if (command == "done" && processedInput.size() == 2) {
        if (processedInput[1] == "all") {
            std::cout << "Marked all tasks as done.\n";
            taskList.clear();
            return;
        }

        int index;
        try {
            index = std::stoi(processedInput[1]) - 1;
        } catch (const std::invalid_argument& e) {
            std::cout << "'done' is followed by the number referring to your task, not the task itself.\n";
            return;
        }

        if (index < 0 || index >= taskList.size()) {
            std::cout << "'done' is followed by the number referring to your task, and is non-zero.\n";
            return;
        }

        taskList.erase(taskList.begin()+index);
        std::cout << "Marked task " << index+1 << " as done.\n";
        return;
    }

    std::cout << "Command '" << command << "' not recognized or you provided invalid number of args.\n";
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