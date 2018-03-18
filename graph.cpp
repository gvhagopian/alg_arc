
#include "graph.hpp"
#include <algorithm>

using namespace std;

AlgArc::Graph::Graph(sf::Vector2f const &res) : 
    screen_area(sf::RectangleShape()),
    grid_dimension(10),
    small_ticks_per_tick(5)
{
    
    if (!sf::Shader::isAvailable() ||
        !axes_shader.loadFromFile("axes.frag", sf::Shader::Fragment))
    {
        
        exit(EXIT_FAILURE);        
    }
    
    resize(res);
    
}



AlgArc::Graph::~Graph() {
    
    
}

void
AlgArc::Graph::resize(sf::Vector2f const &res) {
    
    int smaller_dim = floor(min(res.x, res.y));
    
    int multiplier_adjust = smaller_dim % (grid_dimension * small_ticks_per_tick);
    
    grid_dimension_px = smaller_dim - multiplier_adjust;
    
    screen_area.setSize(sf::Vector2f(grid_dimension_px, grid_dimension_px));
    
    float scaling_factor = grid_dimension_px / (float)grid_dimension;
    
    axes_shader.setParameter("res", res.x, res.y);
    axes_shader.setParameter("tick_lengths", (float)minor_tick_length, (float)major_tick_length);
    axes_shader.setParameter("tick_intervals", 1.0 * scaling_factor, small_ticks_per_tick * scaling_factor);
    
    center(res);
}

void
AlgArc::Graph::center(sf::Vector2f const &res) {
    int larger_dim = floor(max(res.x, res.y));
    int smaller_dim = floor(min(res.x, res.y));
    int multiplier_adjust = smaller_dim % (grid_dimension * small_ticks_per_tick);
    
    int larger_pos = ((larger_dim - smaller_dim) / 2) + multiplier_adjust / 2;
    int smaller_pos = multiplier_adjust / 2;
    
    sf::Vector2f pos = res.x > res.y ? sf::Vector2f(larger_pos, smaller_pos) : sf::Vector2f(smaller_pos, larger_pos);
    
    screen_area.setPosition(pos);
    
}


void
AlgArc::Graph::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    
    target.draw(screen_area, &axes_shader);
    
}
