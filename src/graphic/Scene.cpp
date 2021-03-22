#include "Scene.h"

Scene::Scene():
    m_window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), WIN_TITLE, WIN_FLAGS),
    m_input(m_window)
{
    // center the window on the screen
    const auto desktop = sf::VideoMode::getDesktopMode();
    sf::Vector2i middleScreen(desktop.width/2 - width()/2, desktop.height/2 - height()/2);
    m_window.setPosition(middleScreen);
}

Scene::~Scene() {

}

unsigned int Scene::width() const {
    return m_window.getSize().x;
}

unsigned int Scene::height() const {
    return m_window.getSize().y;
}

void Scene::run(void (*update)(Scene &scene), void (*draw)(Scene &scene)) {
	while(m_window.isOpen()) {
        // Process events
        m_input.update();

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