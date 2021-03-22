#include "Simulation.h"

#include <SFML/System/Clock.hpp>

Simulation::Simulation():
    Simulation(nullptr)
{

}

Simulation::Simulation(shared_ptr<Instruction> recipe):
    m_recipe(nullptr),
    m_pause(true),
    m_elapsedTime(sf::Time::Zero),
    m_map{},
    m_step(0),
    m_hasScene(false),
    m_scene(nullptr)
{
    setRecipe(recipe);
}

Simulation::~Simulation() {

}

void Simulation::start() {
    m_pause = false;
}

void Simulation::stop() {
    m_pause = true;
    m_elapsedTime = sf::Time::Zero;
    m_map = {};
    m_step = 0;
}

void Simulation::pause() {
    m_pause = true;
}

void Simulation::resume() {
    m_pause = false;
}

void Simulation::setRecipe(shared_ptr<Instruction> recipe) {
    m_recipe = recipe;
}

void Simulation::bindWindow(shared_ptr<Scene> scene) {
    m_scene = scene;
}

void Simulation::update() {

}

void Simulation::loop() {
    sf::Clock clock;

    update();

    if(m_hasScene) {
        m_scene->draw(m_map);
    }

    sf::Time t = clock.getElapsedTime();
    m_elapsedTime += t;

    if(!m_pause) {
        loop();
    }
}