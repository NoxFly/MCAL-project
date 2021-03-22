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

        void update();
        void loop();

        void setRecipe(shared_ptr<Instruction> recipe);
        void bindWindow(shared_ptr<Scene> scene);

    private:
        shared_ptr<Instruction> m_recipe;
        bool m_pause;
        sf::Time m_elapsedTime;
        vector<vector<int>> m_map;
        int m_step;
        shared_ptr<Scene> m_scene;
        bool m_hasWindow;
};

#endif // SIMULATION_H