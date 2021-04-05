#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

/**
 * @struct Instruction
 * Stores simulation recipe's data
 * @property steps - a number of steps
 * @property map - a 2D matrix
 * @property rules - some rules that can update cellular automaton
 */
typedef struct {
    std::vector<std::vector<int>> map;
    std::vector<std::vector<int>> rules;
    int steps;
} Instruction;

void printInstruction(std::shared_ptr<Instruction> ins);
bool isValidInstruction(std::shared_ptr<Instruction> instruction);

#endif // INSTRUCTION_H