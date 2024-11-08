#include <SDL2/SDL.h>
#include <GL/gl.h>

#include "includes.hpp"
#include "node.hpp"

class interactiveWindow
{
private:
    SDL_Window* window_; //= nullptr;
    SDL_GLContext context_;

    int gridWidth_;
    int gridHeight_;
    int windowWidth_;
    int windowHeight_;

    float valueY_;
    float valueX_;
    float valueMag_;
    float valueMagBase_ = 0.2;
    float valueNormalized_ = 1.0;

    // GifFileType* gifFile_;
    // const char* gifFileName_;
    // unsigned char* pixels_;
    // ColorMapObject gifColorMap_;
    // int frameCount_; // Keep track of the number of frames saved

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
        float vecMax
    );


    void render
    (
        const std::vector<std::vector<float>>& vecX, 
        const std::vector<std::vector<float>>& vecY,
        float umagMax
    );

    void render
    (
        const std::vector<std::vector<node>>& nodes
    );


    std::tuple<int, int, float, float> fetchCursorMove();



};