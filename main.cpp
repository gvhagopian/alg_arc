#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <iomanip>

#include "fn_driver.hpp"
#include "graph.hpp"
#include <SFML/Graphics.hpp>

#define INIT_RES_X          1024.0
#define INIT_RES_Y          768.0

#define KEY_BACKSPACE       0x08
#define KEY_ENTER           0x0D

using namespace std;

typedef sf::Keyboard KB;

const std::string fn_shader_str_start =
    "float DistToLine(vec2 pt1, vec2 pt2, vec2 testPt) {"
        "vec2 lineDir = pt2 - pt1;"
        "vec2 perpDir = vec2(lineDir.y, -lineDir.x);"
        "vec2 dirToPt1 = pt1 - testPt;"
        "return abs(dot(normalize(perpDir), dirToPt1));"
    "}"
    "float f_x(float x) {"
        "return "; // < user fn inserted here
const std::string fn_shader_str_end =
        ";"
    "}"
    "uniform vec2 res;"
    "uniform float scale;"
    "void main() {"
        "vec2 pixel_origin = vec2(res.x / 2.0f, res.y / 2.0);"
        "vec2 pixel_coord = vec2(gl_FragCoord.x, gl_FragCoord.y);"
        "vec2 grid_coord = pixel_coord - pixel_origin;"
        "vec2 fn_coord = vec2(grid_coord.x, f_x(grid_coord.x));"
        "vec2 fn_coord_prev = vec2(grid_coord.x - 0.01, f_x(grid_coord.x - 0.01));"
        "vec2 fn_coord_next = vec2(grid_coord.x + 0.01, f_x(grid_coord.x + 0.01));"
        "float dist_from_line = abs(grid_coord.y - fn_coord.y);"
        "for (float i = -1.0; i < 1.0; i += 0.001)"
            "dist_from_line = min(dist_from_line, abs((grid_coord.y) - f_x(grid_coord.x + i)));"
        // "dist_from_line = min(dist_from_line, abs((grid_coord.y) - f_x(grid_coord.x - 0.9)));"
        // "dist_from_line = min(dist_from_line, abs((grid_coord.y) - f_x(grid_coord.x - 0.8)));"
        // "dist_from_line = min(dist_from_line, abs((grid_coord.y) - f_x(grid_coord.x - 0.5)));"
        // "dist_from_line = min(dist_from_line, abs((grid_coord.y) - f_x(grid_coord.x - 0.2)));"
        // "dist_from_line = min(dist_from_line, abs((grid_coord.y) - f_x(grid_coord.x + 0.2)));"
        // "dist_from_line = min(dist_from_line, abs((grid_coord.y) - f_x(grid_coord.x + 0.5)));"
        // "dist_from_line = min(dist_from_line, abs((grid_coord.y) - f_x(grid_coord.x + 0.8)));"
        // "dist_from_line = min(dist_from_line, abs((grid_coord.y) - f_x(grid_coord.x + 0.9)));"
        // "dist_from_line = min(dist_from_line, abs((grid_coord.y) - f_x(grid_coord.x + 0.99)));"
        "float diffusion_factor = dist_from_line / (res.y / 20.0);"
        "gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0 - diffusion_factor);"
    "}";

int main(const int argc, const char **argv) {
    
    sf::Vector2f res(INIT_RES_X, INIT_RES_Y);
    
    sf::Font fn_font;
    vector<sf::Shader *> fn_shaders;
    ostringstream fn_shader_str;
    
    AlgArc::Graph g(res);
    
    if (!fn_font.loadFromFile("ProFont.ttf")) {
        cout << "load font failure" << endl;
        return EXIT_FAILURE;
    }
    
    sf::Text fn_txt("", fn_font, 30);
    fn_txt.setStyle(sf::Text::Regular);
    string cur_fn;
    
    
    sf::FloatRect visibleArea(0, 0, res.x, res.y);
    sf::RenderWindow window(sf::VideoMode(res.x, res.y), "Alg Arc");
    
    window.setFramerateLimit(60);
    sf::Clock clock;
    FnParse::Driver driver;
    istringstream parse_stream;
    
    sf::RectangleShape background(res);
    
    window.setKeyRepeatEnabled(true);
    
    sf::VertexArray fn_path(sf::LinesStrip);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                res.x = event.size.width;
                res.y = event.size.height;
                visibleArea = sf::FloatRect(0, 0, res.x, res.y);
                
                g.resize(res);
                // for (vector<sf::Shader>::iterator it = fn_shaders.begin();
                    // it != fn_shaders.end();
                    // ++it)
                // {
                    // it->setParameter("res", res.x, res.y);
                // }
                window.setView(sf::View(visibleArea));
                if (driver.parse_result == 0) {
                    driver.print(cout);
                    // fn_path.resize(res.x);
                    // for (int i = 0, x = res.x / -2; i < res.x; ++i, ++x) {
                        // fn_path[i].position = sf::Vector2f(i, -driver.fnroot->eval(x) + res.y / 2);
                    // }
                }
                break;
            case sf::Event::TextEntered:
                cur_fn = fn_txt.getString();
                if (event.text.unicode == KEY_BACKSPACE) {
                    if (cur_fn.length() > 0) {
                        cur_fn.pop_back();
                        fn_txt.setString(cur_fn);
                    }
                } else if (event.text.unicode == KEY_ENTER) {
                    parse_stream = istringstream(fn_txt.getString());
                    driver.parse(parse_stream);
                    if (driver.parse_result == 0) {
                        cout << "PARSE OK" << endl;
                        fn_shader_str << fixed;
                        fn_shader_str.str("");
                        fn_shader_str << fn_shader_str_start << driver.fnroot << fn_shader_str_end;
                        cout << "TESTING FN PRINT: " << driver.fnroot << endl;
                        cout << "FN:" << endl << fn_shader_str.str() << endl;
                        sf::Shader *new_fn = new sf::Shader();
                        if (new_fn->loadFromMemory(fn_shader_str.str(), sf::Shader::Fragment)) {
                            new_fn->setParameter("res", res.x, res.y);
                            fn_shaders.push_back(new_fn);
                        } else {
                            delete(new_fn);
                        }
                        driver.print(cout);
                        // fn_path.resize(res.x);
                        // for (int i = 0, x = res.x / -2; i < res.x; ++i, ++x) {
                            // fn_path[i].position = sf::Vector2f(i, -driver.fnroot->eval(x) + res.y / 2);
                        // }
                    }
                    
                } else {
                    fn_txt.setString(fn_txt.getString() + event.text.unicode);
                }
                break;
            case sf::Event::KeyPressed:
                break;
            case sf::Event::KeyReleased:
                break;
            }
        }
        
        window.clear();
        
        // window.draw(background, &axes_shader);
        for (vector<sf::Shader *>::iterator it = fn_shaders.begin(); it != fn_shaders.end(); ++it) {
            window.draw(background, *it);
        }
        if (fn_path.getVertexCount() > 0) {
            window.draw(fn_path);
        }
        window.draw(fn_txt);
        
        window.display();
    }
    
    return EXIT_SUCCESS;
}
