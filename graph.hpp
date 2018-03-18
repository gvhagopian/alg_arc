#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__ 1

#include <SFML/Graphics.hpp>

namespace AlgArc {

class Graph : public sf::Drawable {
public:
    
    Graph(sf::Vector2f const &res);
    virtual ~Graph();
    
    void resize(sf::Vector2f const &res);
    void center(sf::Vector2f const &res);
    
    
    sf::Shader axes_shader;
    sf::RectangleShape screen_area;

private:
    
    const int major_tick_length = 4;
    const int minor_tick_length = 2;
    int grid_dimension_px;
    const int grid_dimension;
    const int small_ticks_per_tick;
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

}

#endif