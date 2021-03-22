#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <string>
#include <memory>

#include "Instruction.h"

using namespace std;

class IO_manager {
	public:
		static shared_ptr<Instruction> read_file(string file);
};

#endif // IO_MANAGER_H