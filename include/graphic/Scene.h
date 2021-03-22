#ifndef SCENE_H
#define SCENE_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "win_props.h"

#include "Input.h"

class Scene {
	public:
		Scene();
		~Scene();
        void run(void (*update)(Scene &scene), void (*draw)(Scene &scene));

    private:
        sf::RenderWindow m_window;
        Input m_input;
};

#endif // SCENE_H