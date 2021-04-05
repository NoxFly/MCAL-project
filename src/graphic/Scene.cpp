#include "Scene.h"

#include <SFML/Graphics.hpp>
#include <math.h>

#include "project_settings.h"

Scene::Scene():
    m_window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), WIN_TITLE, WIN_FLAGS),
    m_input(m_window),
    m_views{}
{
    // center the window on the screen
    const auto desktop = sf::VideoMode::getDesktopMode();
    sf::Vector2i middleScreen(desktop.width/2 - width()/2, desktop.height/2 - height()/2);
    m_window.setPosition(middleScreen);

#if CHECK_VIEWS == true
    srand(time(NULL));
#endif
}

Scene::~Scene() {

}

/**
 * Returns either the window is closed or not
 * @return window is still opened
 */
bool Scene::isOpen() const {
    return m_window.isOpen();
}

/**
 * Returns the window's width
 * @return window's width
 */
unsigned int Scene::width() const {
    return m_window.getSize().x;
}

/**
 * Returns the window's height
 * @return window's height
 */
unsigned int Scene::height() const {
    return m_window.getSize().y;
}

/**
 * Adapts the views on the window depending of how many simulations are rendered
 * @param numberOfSimulations the number of simulations
 */
void Scene::adaptView(unsigned int numberOfSimulations) {
    m_views.clear();
    const int n = numberOfSimulations;

#if DEBUG == true
    std::cout << "=== Setting views ===" << std::endl;
#endif

    // calculate the view's repartition through the window
    // I've inverted vny and vnx because I prefer the disposition to be like that
    const int vny = floor(sqrt(n)); // n views on Y-axis
    const int vnx = floor(n / vny); // n views on X-axis

#if DEBUG == true
    printf("vnx & vny : %d & %d\n", vnx, vny);
#endif

    const int sq = vnx * vny; // total number of views in the "regular grid"
    const int pv = n - sq; // potential number of rest views to place on a new ligne

#if DEBUG == true
    printf("pv : %d\n", pv);
#endif

    const float vw = 1 / (float)vnx; // view viewport width
    const float vh = 1 / ((float)vny + (float)pv); // view viewport height
    const float pvw = 1 / (float)(pv==0? 1 : pv); // potential view viewport width

#if DEBUG == true
    printf("vw & vh : %f & %f\npvw : %f\n", vw, vh, pvw);
#endif

    for(unsigned int i=0; i < n; i++) {
        float sx, sy, sw, sh, vx, vy,
              vpx, vpy, vpw, vph;

        sx = sy = 0;
        sw = sh = 500;
        vph = vh;

        // regular grid
        if(i < sq) {
            vpx = (i%vnx) * vw;
            vpy = floor(i/vnx) * vh;
            vpw = vw;
        }

        // last (unregular) row
        else {
            vpx = (i%pv) * pvw;
            vpy = vny * vh;
            vpw = pvw;
        }

        // in px
        sf::View view(sf::FloatRect(sx, sy, sw, sh));

        // in % relativly to the window
        view.setViewport(sf::FloatRect(vpx, vpy, vpw ,vph));

        m_views.push_back(view);
    }
}

/**
 * Resizes the window depending of the given simulation,
 * so it fits the grid's ratio.
 * Intentionally taking the whole screen size
 * @param s the simulation
 */
void Scene::resizeFromSimulation(Simulation &s) {
    const auto desktop = sf::VideoMode::getDesktopMode();
    const sf::Vector2f mapSize = s.getMapSize();
    const float ratio = std::max(mapSize.x, mapSize.y) / std::min(mapSize.x, mapSize.y);
    const unsigned int cellSize = 200; // intentional 'too big' value for cell's size, so the window we'll cover main screen

    unsigned int winW = cellSize * mapSize.x;
    unsigned int winH = cellSize * mapSize.y;

    if(winW >= desktop.width) {
        winW = desktop.width - 100;
        winH = winW * mapSize.x / mapSize.y;
    }
    
    if(winH >= desktop.height) {
        winH = desktop.height - 100;
        winW = winH * mapSize.y / mapSize.x;
    }

    // resize
    m_window.setSize(sf::Vector2u(winW, winH));

    // re-center the resized window
    sf::Vector2i middleScreen(desktop.width/2 - width()/2, desktop.height/2 - height()/2);
    m_window.setPosition(middleScreen);
}

/**
 * Updates the scene's input
 */
void Scene::update() {
    m_input.update();
}


/**
 * Renders simulations on the window
 * @param simulations the vector of simulations
 */
void Scene::render(std::vector<Simulation> &simulations) {
    // to not go out of bounds for one of the two
    unsigned int m = std::min(simulations.size(), m_views.size());

    sf::Uint8 h = 255 / m;

    // clear
    m_window.clear();

    for(int i=0; i < m; i++) {
        const sf::View view = m_views.at(i);
        // focus a view for a simulation
        m_window.setView(view);


#if CHECK_VIEWS == true
        sf::RectangleShape r(view.getSize());
        r.setPosition(0, 0);
        r.setFillColor(sf::Color(h*i, h*i, h*i, 255));
        m_window.draw(r);
#else
        //draw
        draw(simulations.at(i).getMap(), view.getSize());
#endif

    }
    
    // display
    m_window.display();
}

/**
 * Draws a simulation on its view
 * @param map the map to draw
 * @param viewSize the view size
 */
void Scene::draw(const std::vector<std::vector<int>> &map, sf::Vector2f viewSize) {
    // draws a given simulation's map in the current view
    if(map.size() == 0 || map.at(0).size() == 0)
        return;

    const sf::Vector2f mapSize(map.at(0).size(), map.size());
    const int iCellSize = std::min(viewSize.x / mapSize.x, viewSize.y / mapSize.y);
    const sf::Vector2f cellSize(iCellSize, iCellSize);

    for(unsigned int i=0; i < mapSize.y; i++) {
        const std::vector<int> row = map.at(i);

        for(unsigned int j=0; j < mapSize.x; j++) {
            sf::Vector2f position(j * iCellSize, i * iCellSize);
            sf::RectangleShape cell(cellSize);
            cell.setPosition(position);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color(50, 50, 50));

            if(row.at(j) == 1) { // we do not care about states that are higher than 1 for now
                cell.setFillColor(sf::Color(200, 50, 50));
            }

            else {
                cell.setFillColor(sf::Color(10, 10, 10));
            }

            m_window.draw(cell);
        }
    }
}