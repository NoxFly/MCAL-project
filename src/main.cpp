#include <iostream>

#include "project_settings.h"

#include "Scene.h"
#include "IO_manager.h"
#include "Simulation.h"

using namespace std;


void launch(vector<Simulation> &simulations);

int main(int argc, char **argv) {
    if(argc < 2) {
        cout << "Usage : " << argv[0] << " <path_to_file>" << endl;
        return EXIT_FAILURE;
    }

    // stocks every created simulations
    vector<Simulation> simulations{};
    
    for(int i=1; i < argc; i++) {
        // read the recipe for a new simulation
        const shared_ptr<Instruction> ins = IO_manager::read_file(argv[1]);

        if(!ins) {
            return EXIT_FAILURE;
        }

        // create the simulation with given recipe
        simulations.push_back(Simulation(ins));
    }

    // it doens't need to do anything if there's no simulation to play
    if(simulations.size() > 0) {
        launch(simulations);
    }

    return EXIT_SUCCESS;
}



void launch(vector<Simulation> &simulations) {
    cout << "Starting the simulation..." << endl;

    // preparing stuff - starting simulations
    for(auto &s : simulations)
        s.start();

    sf::Clock clock;
    float interval = 1000 / FPS;
    sf::Time then = sf::Time::Zero;

    Scene *scene = nullptr;

    // create a graphic scene to render the simulation
    // if graphic is enabled
    if(USE_GRAPHICS) {
        scene = new Scene();
        // split the scene in multiple views to draw each simulations
        scene->adaptView(simulations.size());
    }

    // main simulation loop
    while(!USE_GRAPHICS || (scene != nullptr && scene->isOpen())) {
        // update the scene's input
        if(USE_GRAPHICS)
            scene->update();
        
        // manage fps
        sf::Time delta = clock.getElapsedTime() - then;
        if(delta.asMilliseconds() > interval) {
            then = clock.getElapsedTime() - (delta % sf::milliseconds(interval));

            // update simulations
            for(auto &s : simulations)
                s.update(clock.getElapsedTime());

            // render
            if(USE_GRAPHICS)
                scene->render(simulations);
        }
    }

    // end - stop simulations
    for(auto &s : simulations)
        s.stop();

    cout << "Simulation ended" << endl;
}