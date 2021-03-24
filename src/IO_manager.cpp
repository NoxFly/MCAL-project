#include "IO_manager.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

shared_ptr<Instruction> IO_manager::read_file(string file) {
    ifstream infile(file);

    if(!infile.good()) {
        cerr << "IO_manager::read_file Error : file not found or can't be opened." << endl;
        infile.close();
        return nullptr;
    }

    shared_ptr<Instruction> recipe = make_shared<Instruction>();
    recipe->map = {};
    recipe->rules = {};
    recipe->steps = 0;

    string line;
    
    if(!getline(infile, line)) {
        cerr << "IO_manager::read_file Error : missing value (steps)." << endl;
        infile.close();
        return nullptr;
    }

    try {
        recipe->steps = stoi(line);
    } catch(exception &err) {
        cerr << "IO_manager::read_file Error : steps given value isn't an integer." << endl;
        infile.close();
        return nullptr;
    }

    if(recipe->steps < -1) {
        cerr << "IO_manager::read_file Warning : cannot set a negative number of steps. Transformed to -1 for infinite steps." << endl;
        recipe->steps = -1;
    }
    
    int readingRules = 0;

    // read line
    while(getline(infile, line) && readingRules < 3) {
        // string.split(" ")
        istringstream iss(line);
        vector<string> result((istream_iterator<string>(iss)), istream_iterator<string>());

        // read a blank before we read rules or map : change section
        if(result.size() == 0) {
            readingRules++;
        }

        else {
            vector<int> numbers{};

            // read each "word" and transform it to a number type
            for(auto sNumber : result) {
                try {
                    numbers.push_back(stoi(sNumber));
                } catch(exception &err) {
                    cerr << "IO_manager::read_file Error while reading "
                         << (readingRules < 2 ? "rules" : "map")
                         << " : read something else than an integer"
                         << endl;
                    infile.close();
                    return nullptr;
                }
            }

            // rules
            if(readingRules == 1) {
                recipe->rules.push_back(numbers);
            }

            // map
            else {
                recipe->map.push_back(numbers);
            }
        }
    }

    infile.close();

    if(!isValidInstruction(recipe)) {
        return nullptr;
    }

    return recipe;
}