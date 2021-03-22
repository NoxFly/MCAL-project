#include "Scene.h"

Scene::Scene():
    m_window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), WIN_TITLE),
    m_input(m_window)
{

}

Scene::~Scene() {

}

void Scene::run(void (*update)(Scene &scene), void (*draw)(Scene &scene)) {
	while(m_window.isOpen()) {
        // Process events
        sf::Event event;
        
        while(m_window.pollEvent(event)) {
            // Close window: exit
            if(event.type == sf::Event::Closed)
                m_window.close();
        }

        // update stuff here...
        update(*this);

        // Clear screen
        m_window.clear();

        // draw stuff here...
        draw(*this);

        // Update the window
        m_window.display();
    }
}