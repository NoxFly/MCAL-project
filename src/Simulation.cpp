#include "Simulation.h"

#include <SFML/System/Clock.hpp>

#include "project_settings.h"

Simulation::Simulation():
    Simulation(nullptr)
{

}

Simulation::Simulation(shared_ptr<Instruction> recipe):
    m_recipe(nullptr),
    m_pause(true),
    m_step(0),
    m_map{}
{
    setRecipe(recipe);
}

Simulation::~Simulation() {

}

void Simulation::start() {
    if(m_recipe == nullptr) {
        cout << "Simulation::start Warning : cannot start the simulation because it has no recipe." << endl;
        return;
    }

    m_pause = false;
    m_step = 0;
    m_map.assign(m_recipe->map.begin(), m_recipe->map.end());
}

void Simulation::stop() {
    m_pause = true;
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

#if DEBUG == true
    printInstruction(m_recipe);
#endif
}

void Simulation::update(sf::Time elapsedTime) {
    if(m_pause) {
        return;
    }

    if(m_recipe == nullptr) {
        cout << "Simulation::update Warning : cannot update the simulation because it has no recipe." << endl;
        return;
    }
}

vector<vector<int>> Simulation::getMap() const {
    return m_map;
}