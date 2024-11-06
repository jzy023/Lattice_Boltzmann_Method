#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <gif_lib.h>

#include "includes.hpp"

class interactiveWindow
{
private:
    SDL_Window* window_; //= nullptr;
    SDL_GLContext context_;

    int gridWidth_;
    int gridHeight_;
    int windowWidth_;
    int windowHeight_;

    // GifFileType* gifFile_;
    // const char* gifFileName_;
    // unsigned char* pixels_;
    // ColorMapObject gifColorMap_;
    // int frameCount_; // Keep track of the number of frames saved

    // private 
    std::vector<float> getJetColor(float normalizedValue);

    // Private method to initialize GIF file
    bool initializeGif(const char* filename);

    // Private method to save a frame to the GIF
    void saveFrameToGif();

    // Private method to close the GIF file
    void closeGif();


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