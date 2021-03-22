#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

using namespace std;

typedef struct {
    std::vector<std::vector<int>> map;
    std::vector<std::vector<int>> rules;
    int steps;
} Instruction;

void printInstruction(shared_ptr<Instruction> ins);
bool isValidInstruction(std::shared_ptr<Instruction> instruction);

#endif // INSTRUCTION_H