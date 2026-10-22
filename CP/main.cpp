#include <iostream>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <stack>

enum Op : uint8_t {
    MOV, MOVI, ADD, ADDI, SUB, SUBI, MUL, MULI, JMP, PRINT, PRINTI, HALT
};

struct Instruction {
    Op op;
    int64_t a = 0;
    int64_t b = 0;
    int64_t c = 0;
};

void run(const std::vector<Instruction>& code) {
    std::stack<int64_t> stack;
    size_t ip = 0;

    int64_t regs[16];
    for (int i = 0; i < 16; ++i)
        regs[i] = 0;

    while (true) {
        //printf("ip=%zu op=%d\n", ip, code[ip].op);
        const auto& ins = code[ip];
        switch (code[ip].op) {
            case MOV: regs[ins.a] = regs[ins.b]; break;
            case MOVI: regs[ins.a] = ins.b; break;
            case ADD: regs[ins.a] += regs[ins.b]; break;
            case ADDI: regs[ins.a] += ins.b; break;
            case SUB: regs[ins.a] -= regs[ins.b]; break;
            case SUBI: regs[ins.a] -= ins.b; break;
            case MUL: regs[ins.a] *= regs[ins.b]; break;
            case MULI: regs[ins.a] *= ins.b; break;
            case JMP: ip = ins.a - 1; break;
            case PRINT: {
                printf("%lld\n", static_cast<long long>(regs[ins.a]));
                break;
            }
            case PRINTI: {
                printf("%lld\n", static_cast<long long>(ins.a));
                break;
            }
            case HALT: return;
        }
        ip++;
    }
}

int main() {
    std::vector<Instruction> program = {
        {PRINTI, 3},
        {PRINTI, 2},
        {PRINTI, 1},
        {JMP, 0},
        {HALT}
    };

    run(program);

    return 0;
}
