#include "Instruction.h"

Instruction::Instruction():
    m_map{}, m_rules{}, m_steps(0)
{

}

Instruction::Instruction(std::vector<std::vector<int>> map, std::vector<int> rules, int steps):
    m_map{map}, m_rules{rules}, m_steps(steps)
{

}

Instruction::~Instruction() {

}