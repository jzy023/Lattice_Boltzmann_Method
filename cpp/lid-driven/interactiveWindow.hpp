#include <SDL2/SDL.h>
#include <GL/gl.h>

#include "../includes.hpp"


class interactiveWindow
{
private:
    SDL_Window* window_; //= nullptr;
    SDL_GLContext context_;

    int gridWidth_;
    int gridHeight_;
    int windowWidth_;
    int windowHeight_;

    // private 
    std::vector<float> getJetColor(float normalizedValue);


public:
    // constructo
    interactiveWindow();

    interactiveWindow
    (
        const char* name,
        int size
    );

    interactiveWindow
    (
        int gridWidth,
        int gridHeight,
        int windowWidth,
        int windowHeight
    );

    // destructor
    ~interactiveWindow();
        

    // getters
    

    // methods
    void render
    (
        const std::vector<std::vector<float>>& vec,
        const float vecMax
    );


    void render
    (
        const std::vector<std::vector<float>>& vecX, 
        const std::vector<std::vector<float>>& vecY,
        const float umagMax
    );


    std::tuple<int, int, float, float> fetchCursorMove();


};