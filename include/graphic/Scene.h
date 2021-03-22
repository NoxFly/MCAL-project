#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "win_props.h"
#include "Input.h"


using namespace std;

class Scene {
	public:
		Scene();
		~Scene();
        
        void draw(vector<vector<int>> map);

        // getters
        unsigned int width() const;
        unsigned int height() const;

    private:
        sf::RenderWindow m_window;
        Input m_input;
};

#endif // SCENE_H