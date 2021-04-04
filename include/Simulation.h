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
        friend ostream& operator<<(ostream& os, const Simulation& s);

    private:
        vector<int> around(int abs,int ord);
        int modulo(int a,int b);
        int getNextState(int state, vector<int> neighbors);

        shared_ptr<Instruction> m_recipe;
        bool m_pause;
        vector<vector<int>> m_map;
        int m_step;
        map<int, map<int, int>> m_conditions;
};

#endif // SIMULATION_H