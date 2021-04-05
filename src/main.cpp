#include <iostream>

#include "project_settings.h"

#include "Scene.h"
#include "IO_manager.h"
#include "Simulation.h"


void launch(std::vector<Simulation> &simulations);

/**
 * Main
 */
int main(int argc, char **argv) {
    // you have to specify at least one recipe
    // otherwise it's completly useless to run the program without any simulation
    if(argc < 2) {
        std::cout << "Usage : " << argv[0] << " <path_to_file>" << std::endl;
        return EXIT_FAILURE;
    }

    // stocks every created simulations
    std::vector<Simulation> simulations{};

    int j = argc;

#if CHECK_VIEWS == true
    j = 10; // create exactly 10 same views to check view's disposition
#endif
    
    // for each file given
    for(int i=1; i < j; i++) {
        char* arg = argv[i];

#if CHECK_VIEWS == true
        arg = argv[1];
#endif

        // read the recipe for a new simulation
        const std::shared_ptr<Instruction> ins = IO_manager::read_file(arg);

        // recipe is not well formed : exit
        // the reason is explained in the read_file() function
        if(!ins)
            return EXIT_FAILURE;

        // create the simulation with given recipe
        simulations.push_back(Simulation(ins));
    }

    // it doens't need to do anything if there's no simulation to play
    if(simulations.size() > 0)
        launch(simulations);

    return EXIT_SUCCESS;
}


/**
 * Starts simulations and process the main loop
 * - update simulations
 * - render simulations
 * @param simulations a vector storing simulations
 */
void launch(std::vector<Simulation> &simulations) {
    std::cout << "Starting the simulation..." << std::endl;

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

        if(simulations.size() == 1) {
            // if we only draw 1 simulation, it's easy to resize the window to fill the view
            scene->resizeFromSimulation(simulations[0]);
        }


        // split the scene in multiple views to draw each simulations
        scene->adaptView(simulations.size());

        // render a first time before the first update
        scene->render(simulations);
    }

    else {
        std::cout << "Tip : Press [Enter] to step forward, Ctrl+C to exit." << std::endl;

        // render a first time before the first update
        for(auto &s : simulations)
            std::cout << s;
    }


    // main simulation loop
    while(!USE_GRAPHICS || (scene != nullptr && scene->isOpen())) {
        // update the scene's input
        if(USE_GRAPHICS)
            scene->update();
        
        // manage fps
        sf::Time delta = clock.getElapsedTime() - then;

        if(!USE_GRAPHICS || delta.asMilliseconds() > interval) {
            then = clock.getElapsedTime() - (delta % sf::milliseconds(interval));

            // update simulations
            for(auto &s : simulations)
                s.update(clock.getElapsedTime());

            // render
            if(USE_GRAPHICS)
                scene->render(simulations);
            
            // output on terminal : wait for the user to press Enter key...
            else {
                for(auto &s : simulations)
                    std::cout << s;

                std::cin.ignore();
                std::cout << "Next step :" << std::endl;
            }
        }
    }

    // end - stop simulations
    for(auto &s : simulations)
        s.stop();

    std::cout << "Simulation ended" << std::endl;
}