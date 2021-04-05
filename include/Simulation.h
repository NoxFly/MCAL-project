#ifndef SIMULATION_H
#define SIMULATION_H

#include "Instruction.h"
#include "Scene.h"

#include <SFML/System/Time.hpp>

class Simulation {
	public:
		Simulation();
		Simulation(std::shared_ptr<Instruction> recipe);
		~Simulation();

        void start();
        void stop();
        void pause();
        void resume();

        void update(sf::Time elapsedTime);
        void setRecipe(std::shared_ptr<Instruction> recipe);
        std::vector<std::vector<int>> getMap() const;
        sf::Vector2f getMapSize() const;

        friend std::ostream& operator<<(std::ostream& os, const Simulation& s);

    private:
        std::vector<int> around(int abs, int ord);
        int modulo(int a, int b);
        int getNextState(int state, std::vector<int> neighbors);

        std::shared_ptr<Instruction> m_recipe;
        bool m_pause;
        std::vector<std::vector<int>> m_map;
        int m_step;
        std::map<int, std::map<int, int>> m_conditions;
};

#endif // SIMULATION_H