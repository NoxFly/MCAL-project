#include "Simulation.h"

#include <SFML/System/Clock.hpp>

#include "project_settings.h"

/**
 * Default constructor - no recipe provided
 */
Simulation::Simulation(): Simulation(nullptr) {}

/**
 * Creates a simulation with given recipe
 */
Simulation::Simulation(std::shared_ptr<Instruction> recipe):
    m_recipe(nullptr),
    m_pause(true),
    m_step(0),
    m_map{},
    m_conditions{}
{
    setRecipe(recipe);
}

/**
 * Destructor
 */
Simulation::~Simulation() {

}

/**
 * Starts the simulation.
 * Throws an error if simulation haven't recipe yet.
 * Initialize variables.
 */
void Simulation::start() {
    if(m_recipe == nullptr) {
        std::cout << "Simulation::start Warning : cannot start the simulation because it has no recipe." << std::endl;
        return;
    }

    m_pause = false;
    m_step = m_recipe->steps > -1 ? 0 : -1;
    m_map.assign(m_recipe->map.begin(), m_recipe->map.end());
}

/**
 * Stops the running simulation.
 * Reset variables.
 */
void Simulation::stop() {
    m_pause = true;
    m_map = {};
    m_step = m_recipe->steps > -1 ? 0 : -1;
}

/**
 * Pauses the simulation
 */
void Simulation::pause() {
    m_pause = true;
}

/**
 * Resumes the simulation
 */
void Simulation::resume() {
    m_pause = false;
}

/**
 * Bind a recipe to the simulation
 * Also interpret the rules to 2D map (explained in the function)
 * @param recipe recipe to bind
 */
void Simulation::setRecipe(std::shared_ptr<Instruction> recipe) {
    m_recipe = recipe;

#if DEBUG == true
    printInstruction(m_recipe);
#endif

    /* bind rules conditions
       Q -> [conditions] -> Q'

       Map<int, Map<int, int>> = {
           Q: {
               NeighborsNumber: Q'
           }
       }
    */

   m_conditions.clear();

   for(auto rule : m_recipe->rules) {
       const int Q = rule[0];
       const int Qp = rule[rule.size()-1];

        if(!m_conditions.count(Q))
            m_conditions[Q] = {};

        if(rule.size() > 2) {
            // this rule has conditions to apply from Q to Q'
            /*
            1 0 1 1 1  (a rule : if a cell beeing 1 has 2 or 3 neighbors, keep its state)
              1 2 3    (number of neighbors)
            Q   ^ ^ Qp

            m_conditions[1] = {
                2: 1
                3: 1
            }

            m_conditions[2] = {
                2: 1
                3: 1
            }

            But we're not doing like that. Instead, we do not write a rule for a state that finally will result to the same one
            so, instead of
            1 0 1 1 1, meaning an alive cell with 2 or 3 neighbors stays alive
            we do the "negative" rule because this one makes a change :
            1 1 0 0 1 1 1 1 1 0, meaning an alive cell with less or more than 2 or 3 neighbors dies.
            For an empty place or dead cell, we do :
            0 0 0 1 1, meaning that is spawning a cell if an empty place is surrounded by exactly 3 alive cells
            Q 1 2 3 Q'
                  ^ 3 neighbors. All further "0" at the right are not necessary


            For now interpreted rules are limited in the case we have more than 2 states.
            But we can easily update it.

            */
            for(int i=1; i < rule.size()-1; i++) {
                if(rule[i] > 0) {
                    m_conditions[Q][i] = Qp;
                }
            }
        }
   }

// display interpreted rule's map
#if DEBUG == true
    std::cout << "m_conditions = {\n";

    for(auto q : m_conditions) {
        std::cout << "  " << q.first << " : {";
        
        if(q.second.size() > 0)
            std::cout << "\n";

        for(auto qp : q.second) {
            std::cout << "    " << qp.first << " : " << qp.second << "\n";
        }

        if(q.second.size() > 0)
            std::cout << "  ";

        std::cout << "}\n";
    }

    std::cout << "}" << std::endl;
#endif
}


/**
 * Returns the next state (Q') of a given state (Q) following its neighbors.
 * It uses the Moore's neighborhood (takes the diagonals, and can store more than one range)
 */
int Simulation::getNextState(int state, std::vector<int> neighbors) {
    // limitation :
    // we're not looking for specific neighbor's state, but ony the number of neighbors
    // so we count all neighbors with state of 0, thus we substract the total of neighbors by it
    // to get the number of neighbors with a state > 0
    const int n = neighbors.size() - count(neighbors.begin(), neighbors.end(), 0);

    // if the state of the cell Q is found as a key in the rules
    if(m_conditions.count(state)) {
        // if we find the [condition] in the sub-map for the given state's rule
        if(m_conditions[state].count(n)) {
            // return Q'
            return m_conditions[state][n];
        }
    }

    return state; // no rule found = no changes provided
}


/**
 * Returns the modulo of a number a by b.
 * Takes care of negative bounds.
 * C++ : -1%5 = -1
 * Simulation::modulo : -1%5 = 4
 * @param a left operand
 * @param b right operand
 * @return a%b result
 */
int Simulation::modulo(int a, int b) {
    return a >= b ? a - b : a < 0 ? b + a : a;
}

/**
 * Returns a vector of neighbors around a given cell
 * @param abs cell's X-axis
 * @param ord cell's Y-axis
 * @return the vector of neighbors, in the order from top-left to bottom-right
 */
std::vector<int> Simulation::around(int abs, int ord) {
    std::vector<int> near = {};
    int x, y;
    const int range = 1; // for now we're limiting to only one range

    for(int i = -range; i <= range; i++) {
        for(int j = -range; j <= range; j++) {
            if(i == j && i == 0) // to not take care of the cell itself
                continue;
            
            x = modulo(abs + j, m_map[0].size());
            y = modulo(ord + i, m_map.size());
            
            near.push_back(m_map[y][x]);
        }
    }

    return near;
}

/**
 * Updates the simulation.
 * If no recipe binded, if in pause or if all steps executed, it doesn't update.
 * updates the states of each cells in the matrix.
 * @param elapsedTime (unused for now) the elapsedTime in the program main's loop
 */
void Simulation::update(sf::Time elapsedTime) {
    if(m_recipe == nullptr) {
        std::cout << "Simulation::update Warning : cannot update the simulation because it has no recipe." << std::endl;
        return;
    }

    if(m_pause || (m_step > -1 && m_step >= m_recipe->steps)) {
        return;
    }

    // we can't directly update the matrix else it will
    // wrongly process for next cells that will be updated after the first one
    // so we create the "next gen" matrix
    std::vector<std::vector<int>> tmp(m_map.size(), std::vector(m_map[0].size(), 0));


    for(int y=0; y < m_map.size(); y++) {
        for(int x=0; x < m_map[y].size(); x++) {
            std::vector<int> neigh = around(x, y);

#if DEBUG == true
            printf("(%i, %i) : ", x, y);
            for(auto k : neigh)
                std::cout << k << ", ";
            std::cout << std::endl;
#endif

            int nextState = getNextState(m_map[y][x], neigh);
            tmp[y][x] = nextState;
        }
    }

    // update matrix
    m_map.assign(tmp.begin(), tmp.end());

    // if it's not infinite steps, increment it
    if(m_step > -1)
        m_step++;
}

/**
 * Returns the current map of the simulation
 * @return simulation's map
 */
std::vector<std::vector<int>> Simulation::getMap() const {
    return m_map;
}

/**
 * Returns the map's size
 * @return map's size
 */
sf::Vector2f Simulation::getMapSize() const {
    return sf::Vector2f(
        m_map.size() > 0 ? m_map[0].size() : 0,
        m_map.size()
    );
}

/**
 * Prints the given simulation in the given stream
 * @param os output stream
 * @param s simulation to print
 * @return updated output stream
 */
std::ostream& operator<<(std::ostream& os, const Simulation& s) {
    for(auto row : s.getMap()) {
        for(auto col : row) {
            os << col;
        }
        os << "\n";
    }

    os << std::endl;

    return os;
}