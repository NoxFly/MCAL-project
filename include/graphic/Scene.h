#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "win_props.h"
#include "Input.h"
#include "Simulation.h"

using namespace std;

class Simulation;

class Scene {
	public:
		Scene();
		~Scene();
        
        void render(vector<Simulation> &simulations);
        bool isOpen() const;
        void update();
        void adaptView(unsigned int numberOfSimulations);

        // getters
        unsigned int width() const;
        unsigned int height() const;

    private:
        sf::RenderWindow m_window;
        Input m_input;
        vector<sf::View> m_views;

        void draw(const vector<vector<int>> &map, sf::Vector2f viewSize);
};

#endif // SCENE_H