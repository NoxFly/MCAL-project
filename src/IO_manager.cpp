#include "IO_manager.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

std::shared_ptr<Instruction> IO_manager::read_file(std::string file) {
    std::ifstream infile(file);

    if(!infile.good()) {
        cout << "Error, file not found or can't be opened" << endl;
        infile.close();
        return nullptr;
    }


    std::string line;

    while(std::getline(infile, line)) {
        std::istringstream iss(line);
        std::vector<std::string> result((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());


        for(unsigned int i=0; i < result.size(); i++) {
            std::cout << result[i] << std::endl;
        }
    }

    infile.close();

    return std::make_shared<Instruction>();
}