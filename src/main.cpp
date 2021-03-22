#include <iostream>

#include "project_settings.h"

#include "Scene.h"
#include "IO_manager.h"

using namespace std;


void update(Scene &scene);
void draw(Scene &scene);


int main(int argc, char **argv) {
    if(argc != 2) {
        cout << "Usage : ./prog <path_to_file>" << endl;
        return EXIT_FAILURE;
    }
    
    std::shared_ptr<Instruction> ins = IO_manager::read_file(argv[1]);


    if(USE_GRAPHICS) {
        Scene scene = Scene();
        scene.run(&update, &draw);
    }

    return EXIT_SUCCESS;
}



void update(Scene &scene) {

}

void draw(Scene &scene) {

}