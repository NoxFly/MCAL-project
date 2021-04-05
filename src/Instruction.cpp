#include "Instruction.h"

/**
 * Prints the recipe's struct in the console
 * @param ins the recipe to print
 */
void printInstruction(std::shared_ptr<Instruction> ins) {
    if(!ins)
        return;

    std::cout << "Steps : " << ins->steps << std::endl;

    std::cout << "\nRules : \n";
    for(auto v : ins->rules) {
        for(auto n : v)
            std::cout << n;
        std::cout << std::endl;
    }

    std::cout << "\nMap " << ins->map.at(0).size() << "x" << ins->map.size() <<  " : \n";
    for(auto v : ins->map) {
        for(auto n : v)
            std::cout << n;
        std::cout << std::endl;
    }
}

/**
 * Returns either the given instruction / recipe is valid or not
 * @param instruction the recipe to check its validity
 * @return recipe's validity
 */
bool isValidInstruction(std::shared_ptr<Instruction> instruction) {
    // steps must be >= -1
    if(instruction->steps < -1) {
        std::cerr << "isValidInstruction() Error : instruction must have 0 or more steps, or -1 for infinite steps." << std::endl;
        return false;
    }

    // no rules specified
    if(instruction->rules.size() == 0) {
        std::cerr << "isValidInstruction() Error : no rule provided." << std::endl;
        return false;
    }

    // no map specified
    if(instruction->map.size() == 0) {
        std::cerr << "isValidInstruction() Error : no map provided." << std::endl;
        return false;
    }

    int ruleLength = instruction->rules[0].size();

    for(auto v : instruction->rules) {
        /* if(v.size() != ruleLength) {
            cerr << "isValidInstruction() Error : rules have not the same size." << endl;
            return false;
        } */

        // a rule must at least have "Q Q'"
        if(v.size() < 2)
            std::cerr << "isValidInstruction() Error : a rule must at least indicates a Q and Q' states." << std::endl;

        // a rule only can contains positive numbers
        for(auto n : v) {
            if(n < 0) {
                std::cerr << "isValidInstruction() Error : negative number not allowed." << std::endl;
                return false;
            }
        }
    }

    int mapWidth = instruction->map[0].size();

    for(auto v : instruction->map) {
        // map must be well filled
        if(v.size() != mapWidth) {
            std::cerr << "isValidInstruction() Error : map rows have not the same size." << std::endl;
            return false;
        }

        /* for(auto n : v) {
            if(n < 0 || n > 1) {
                cerr << "isValidInstruction() Error : map values must be 0 or 1 only." << endl;
                return false;
            }
        } */
    }

    return true;
}