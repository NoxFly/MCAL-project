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
    m_map{},
    m_conditions{}
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
    m_step = m_recipe->steps;
    m_map.assign(m_recipe->map.begin(), m_recipe->map.end());
}

void Simulation::stop() {
    m_pause = true;
    m_map = {};
    m_step = m_recipe->steps;
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

            */
            for(int i=1; i < rule.size()-1; i++) {
                if(rule[i] > 0) {
                    m_conditions[Q][i] = Qp;
                }
            }
        }
   }

#if DEBUG == true
    cout << "m_conditions = {\n";

    for(auto q : m_conditions) {
        cout << "  " << q.first << " : {";
        
        if(q.second.size() > 0)
            cout << "\n";

        for(auto qp : q.second) {
            cout << "    " << qp.first << " : " << qp.second << "\n";
        }

        if(q.second.size() > 0)
            cout << "  ";

        cout << "}\n";
    }

    cout << "}" << endl;
#endif
}


int Simulation::getNextState(int state, vector<int> neighbors) {
    const int n = count(neighbors.begin(), neighbors.end(), 1);

    if(m_conditions.count(state)) {
        if(m_conditions[state].count(n)) {
            return m_conditions[state][n];
        }
    }

    return state; // no changes provided
}



int Simulation::modulo(int a, int b) {
    return a >= b ? a - b : a < 0 ? b + a : a;
}

vector<int> Simulation::around(int abs, int ord) {
    vector<int> near = {};
    int x, y;

    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i == j && i == 0)
                continue;
            
            x = modulo(abs + j, m_map[0].size());
            y = modulo(ord + i, m_map.size());
            
            near.push_back(m_map[y][x]);
        }
    }

    return near;
}

void Simulation::update(sf::Time elapsedTime) {
    if(m_recipe == nullptr) {
        cout << "Simulation::update Warning : cannot update the simulation because it has no recipe." << endl;
        return;
    }

    if(m_pause || (m_step > -1 && m_step >= m_recipe->steps)) {
        return;
    }

    vector<vector<int>> tmp(m_map.size(), vector(m_map[0].size(), 0));


    for(int y=0; y < m_map.size(); y++) {
        for(int x=0; x < m_map[y].size(); x++) {
            vector<int> neigh = around(x, y);

#if DEBUG == true
            printf("(%i, %i) : ", x, y);
            for(auto k : neigh)
                cout << k << ", ";
            cout << endl;
#endif

            int nextState = getNextState(m_map[y][x], neigh);
            tmp[y][x] = nextState;
        }
    }

    m_map.assign(tmp.begin(), tmp.end());

    if(m_step > -1)
        m_step++;
}

vector<vector<int>> Simulation::getMap() const {
    return m_map;
}

ostream& operator<<(ostream& os, const Simulation& s) {
    for(auto row : s.getMap()) {
        for(auto col : row) {
            os << col;
        }
        os << "\n";
    }

    os << endl;

    return os;
}