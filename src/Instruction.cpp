#include "Instruction.h"

void printInstruction(shared_ptr<Instruction> ins) {
    if(!ins) {
        return;
    }

    cout << "Steps : " << ins->steps << endl;

    cout << "\nRules : \n";
    for(auto v : ins->rules) {
        for(auto n : v) {
            cout << n;
        }
        cout << endl;
    }

    cout << "\nMap " << ins->map.at(0).size() << "x" << ins->map.size() <<  " : \n";
    for(auto v : ins->map) {
        for(auto n : v) {
            cout << n;
        }
        cout << endl;
    }
}

bool isValidInstruction(shared_ptr<Instruction> instruction) {
    if(instruction->steps < -1) {
        cerr << "isValidInstruction() Error : instruction must have 0 or more steps, or -1 for infinite steps." << endl;
        return false;
    }

    if(instruction->rules.size() == 0) {
        cerr << "isValidInstruction() Error : no rule provided." << endl;
        return false;
    }

    if(instruction->map.size() == 0) {
        cerr << "isValidInstruction() Error : no map provided." << endl;
        return false;
    }

    int ruleLength = instruction->rules[0].size();

    for(auto v : instruction->rules) {
        /* if(v.size() != ruleLength) {
            cerr << "isValidInstruction() Error : rules have not the same size." << endl;
            return false;
        } */

        if(v.size() < 2) {
            cerr << "isValidInstruction() Error : a rule must at least indicates a Q and Q' states." << endl;
        }

        for(auto n : v) {
            if(n < 0) {
                cerr << "isValidInstruction() Error : negative number not allowed." << endl;
                return false;
            }
        }
    }

    int mapWidth = instruction->map[0].size();

    for(auto v : instruction->map) {
        if(v.size() != mapWidth) {
            cerr << "isValidInstruction() Error : map rows have not the same size." << endl;
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