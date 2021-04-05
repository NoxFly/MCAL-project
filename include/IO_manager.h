#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <string>
#include <memory>

#include "Instruction.h"

class IO_manager {
	public:
		static std::shared_ptr<Instruction> read_file(std::string file);
};

#endif // IO_MANAGER_H