#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>
#include <iostream>
#include <fstream>

class Instruction {
	public:
		Instruction(/* fstream file */);
		~Instruction();
        
        // getter and setter
        std::vector<std::vector<int>> map() const;
        void map(const std::vector<std::vector<int>> newMap);

        std::vector<int> rules() const;
        void rules(const std::vector<int> newRules);

        int steps() const;
        void steps(const int nbStep);

    private:
		std::vector<std::vector<int>> m_map;
		std::vector<int> m_rules;
		int m_steps;
};

#endif // INSTRUCTION_H