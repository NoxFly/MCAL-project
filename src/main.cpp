#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main(int argc, char **argv) {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Automate Cellulaire - projet MCAL");

	while(window.isOpen()) {
        // Process events
        sf::Event event;
        while(window.pollEvent(event)) {
            // Close window: exit
            if(event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear();

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}