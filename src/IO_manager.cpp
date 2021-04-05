#include "IO_manager.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

/**
 * Reads a file, and tries to create a simulation's recipe thanks it
 * @param file path of recipe
 * @return the created recipe. nullptr if something went wrong
 */
std::shared_ptr<Instruction> IO_manager::read_file(std::string file) {
    std::ifstream infile(file);

    // cannot open the file
    if(!infile.good()) {
        std::cerr << "IO_manager::read_file Error : file not found or can't be opened." << std::endl;
        infile.close();
        return nullptr;
    }


    std::shared_ptr<Instruction> recipe = std::make_shared<Instruction>();
    recipe->map = {};
    recipe->rules = {};
    recipe->steps = 0;

    std::string line;
    

    // no content
    if(!getline(infile, line)) {
        std::cerr << "IO_manager::read_file Error : missing value (steps)." << std::endl;
        infile.close();
        return nullptr;
    }

    // try to recover the number of steps : convert string to int
    try {
        recipe->steps = stoi(line);
    }
    catch(std::exception &err) {
        std::cerr << "IO_manager::read_file Error : steps given value isn't an integer." << std::endl;
        infile.close();
        return nullptr;
    }

    // bring back negative numbers to -1
    if(recipe->steps < -1) {
        std::cerr << "IO_manager::read_file Warning : cannot set a negative number of steps. Transformed to -1 for infinite steps." << std::endl;
        recipe->steps = -1;
    }
    
    int readingRules = 0;

    // read line
    while(getline(infile, line) && readingRules < 3) {
        // string.split(" ")
        std::istringstream iss(line);
        std::vector<std::string> result((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

        // read a blank before we read rules or map : change section
        if(result.size() == 0)
            readingRules++; // now read matrix, because we just read a blank line

        else {
            std::vector<int> numbers{};

            // read each "word" and transform it to a number type
            for(auto sNumber : result) {
                try {
                    numbers.push_back(stoi(sNumber));
                }
                catch(std::exception &err) {
                    std::cerr << "IO_manager::read_file Error while reading "
                         << (readingRules < 2 ? "rules" : "map")
                         << " : read something else than an integer"
                         << std::endl;
                    infile.close();
                    return nullptr;
                }
            }

            // rules
            if(readingRules == 1)
                recipe->rules.push_back(numbers);
            // map
            else
                recipe->map.push_back(numbers);
        }
    }

    infile.close();

    if(!isValidInstruction(recipe)) {
        return nullptr;
    }

    return recipe;
}