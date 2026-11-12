#include <functional>
#include <string>
#include <iostream>
#include <vector>

class Command {
public:
    using ExecuteFunction = std::function<void(const std::vector<std::string>& args, std::vector<std::string>& tasks)>;

    Command(const std::string& name, ExecuteFunction execfunc)
        : name(name), execfunc(execfunc) {}
    
    void Execute(const std::vector<std::string>& args, std::vector<std::string>& tasks) const {
        if (execfunc) execfunc(args, tasks);
    }

    bool IsCommand(const std::string& cmdname) { return cmdname == name; }
    
private:
    std::string name;
    ExecuteFunction execfunc;
};

void List(const std::vector<std::string>& args, std::vector<std::string>& tasks) {
    if (!args.empty()) {
        std::cout << "'list' command doesn't take any args, and so any args were ignored.\n";
    }

    for (int i = 0; i < tasks.size(); ++i)
        std::cout << "[" << i+1 << "] " << tasks[i] << "\n";
}

void Add(const std::vector<std::string>& args, std::vector<std::string>& tasks) {
    if (args.empty()) {
        std::cout << "'add' command needs one argument for naming the task.\n";
        return;
    }

    tasks.push_back(args[0]);
}

void Done(const std::vector<std::string>& args, std::vector<std::string>& tasks) {
    if (args.empty()) {
        std::cout << "'done' command needs one argument as an index (int) to delete a task.\n";
        return;
    }

    int index;
    try {
        index = std::stoi(args[0])-1;
    } catch (const std::exception& e) {
        std::cout << "'done' command needs one argument as an index (int) to delete a task (and please not some ridiculous number either).\n";
        return;
    }

    if (index < 0 || index >= tasks.size()) {
        std::cout << "'done' command needs one argument as an index (int) to delete a task, and it has to correspond to a task.\n";
        return;
    }

    tasks.erase(tasks.begin()+index);
}

static std::vector<Command> commands = {
    Command("list", List),
    Command("add", Add),
    Command("done", Done)
};
