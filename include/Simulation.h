#ifndef SIMULATION_H
#define SIMULATION_H

#include "Instruction.h"
#include "Scene.h"

#include <SFML/System/Time.hpp>

using namespace std;

class Simulation {
	public:
		Simulation();
		Simulation(shared_ptr<Instruction> recipe);
		~Simulation();

        void start();
        void stop();
        void pause();
        void resume();

        void update(sf::Time elapsedTime);
        void setRecipe(shared_ptr<Instruction> recipe);
        vector<vector<int>> getMap() const;

    private:
        shared_ptr<Instruction> m_recipe;
        bool m_pause;
        vector<vector<int>> m_map;
        int m_step;
};

#endif // SIMULATION_H