#include "interactiveWindow.hpp"


// constructors
interactiveWindow::interactiveWindow()
:
    gridWidth_(66),
    gridHeight_(66),
    windowWidth_(800),
    windowHeight_(800)
{
    window_ = nullptr; 

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "ERROR!" << std::endl;
        abort();
    }

    // Setup OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    window_ = SDL_CreateWindow("Velocity Field Heatmap", 
                               SDL_WINDOWPOS_UNDEFINED, 
                               SDL_WINDOWPOS_UNDEFINED, 
                               windowWidth_, windowHeight_, SDL_WINDOW_OPENGL);
    
    if (!window_) {
        std::cerr << "ERROR!" << std::endl;
        SDL_Quit();
        abort();
    }

    context_ = SDL_GL_CreateContext(window_);
    if (!context_) {
        std::cerr << "ERROR!" << std::endl;
        SDL_DestroyWindow(window_);
        SDL_Quit();
        abort();
    }

    // initializeGif("0.gif");

    SDL_GL_SetSwapInterval(1);  // Enable VSync
    glOrtho(0.0, gridWidth_, 0.0, gridHeight_, -1.0, 1.0);  // Set orthographic projection
}


interactiveWindow::interactiveWindow
(
    const char* name,
    int size
)
:
    gridWidth_(size),
    gridHeight_(size),
    windowWidth_(800),
    windowHeight_(800)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "ERROR!" << std::endl;
        abort();
    }

    // Setup OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    window_ = SDL_CreateWindow(name, 
                               SDL_WINDOWPOS_UNDEFINED, 
                               SDL_WINDOWPOS_UNDEFINED, 
                               windowWidth_, windowHeight_, SDL_WINDOW_OPENGL);

    if (!window_) {
        std::cerr << "ERROR!" << std::endl;
        SDL_Quit();
        abort();
    }

    context_ = SDL_GL_CreateContext(window_);
    if (!context_) {
        std::cerr << "ERROR!" << std::endl;
        SDL_DestroyWindow(window_);
        SDL_Quit();
        abort();
    }

    // std::string nameGifStr = std::string(name) + ".gif";
    // const char* nameGif = nameGifStr.c_str();
    // initializeGif(nameGif);

    SDL_GL_SetSwapInterval(1);  // Enable VSync
    glOrtho(0.0, gridWidth_, 0.0, gridHeight_, -1.0, 1.0);  // Set orthographic projection
}


interactiveWindow::interactiveWindow
(
    int gridWidth,
    int gridHeight,
    int windowWidth,
    int windowHeight
)
:
    gridWidth_(gridWidth),
    gridHeight_(gridHeight),
    windowWidth_(windowWidth),
    windowHeight_(windowHeight)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "ERROR!" << std::endl;
        abort();
    }

    // Setup OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    window_ = SDL_CreateWindow("Velocity Field Heatmap", 
                               SDL_WINDOWPOS_UNDEFINED, 
                               SDL_WINDOWPOS_UNDEFINED, 
                               windowWidth_, windowHeight_, SDL_WINDOW_OPENGL);

    if (!window_) {
        std::cerr << "ERROR!" << std::endl;
        SDL_Quit();
        abort();
    }

    context_ = SDL_GL_CreateContext(window_);
    if (!context_) {
        std::cerr << "ERROR!" << std::endl;
        SDL_DestroyWindow(window_);
        SDL_Quit();
        abort();
    }

    SDL_GL_SetSwapInterval(1);  // Enable VSync
    glOrtho(0.0, gridWidth_, 0.0, gridHeight_, -1.0, 1.0);  // Set orthographic projection
}


// destructor
interactiveWindow::~interactiveWindow()
{
    // closeGif();

    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}


// private methods
std::vector<float> interactiveWindow::getJetColor
(
    float normalizedValue
)
{
    std::vector<float> color(3, 0.0);  // RGB
    const float r = std::min(1.0f, std::max(0.0f, 1.5f - std::abs(4.0f * normalizedValue - 3.0f)));
    const float g = std::min(1.0f, std::max(0.0f, 1.5f - std::abs(4.0f * normalizedValue - 2.0f)));
    const float b = std::min(1.0f, std::max(0.0f, 1.5f - std::abs(4.0f * normalizedValue - 1.0f)));
    color[0] = r;  // Red
    color[1] = g;  // Green
    color[2] = b;  // Blue
    return color;
}



// methods
void interactiveWindow::render
(
    const std::vector<std::vector<float>>& vec,
    float vecMax
)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);

    for (int iy = 0; iy < gridHeight_ - 1;  iy++) {
        for (int ix = 0; ix < gridWidth_ - 1;  ix++) {

            valueMagBase_ = vecMax;
            valueNormalized_ = vec[iy][ix] / valueMagBase_;

            // Get the corresponding Jet colormap color
            std::vector<float> color = getJetColor(valueNormalized_);

            // Set color using the Jet colormap
            glColor3f(color[0], color[1], color[2]);

            // Draw each grid cell as a square
            glVertex2f(ix, iy);
            glVertex2f(ix, iy + 1);
            glVertex2f(ix + 1, iy + 1);
            glVertex2f(ix + 1, iy);
        }
    }

    glEnd();
    SDL_GL_SwapWindow(window_); 
}



void interactiveWindow::render
(
    const std::vector<std::vector<float>>& vecX, 
    const std::vector<std::vector<float>>& vecY,
    float vecMagMax
)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);

    for (int iy = 0; iy < gridHeight_ - 1;  iy++) {
        for (int ix = 0; ix < gridWidth_ - 1;  ix++) {
            // Compute the velocity magnitude
            valueMag_ = sqrt(vecX[iy][ix] * vecX[iy][ix] + vecY[iy][ix] * vecY[iy][ix]);

            // Normalize velocity for Jet colormap  
            valueMagBase_ = (valueMagBase_ < vecMagMax) * vecMagMax + (valueMagBase_ >= vecMagMax) * valueMagBase_;
            float valueNormalized_ = valueMag_ / valueMagBase_;

            // Get the corresponding Jet colormap color
            std::vector<float> color = getJetColor(valueNormalized_);

            // Set color using the Jet colormap
            glColor3f(color[0], color[1], color[2]);

            // Draw each grid cell as a square
            glVertex2f(ix, iy);
            glVertex2f(ix, iy + 1);
            glVertex2f(ix + 1, iy + 1);
            glVertex2f(ix + 1, iy);
        }
    }
    
    glEnd();
    SDL_GL_SwapWindow(window_);
}



void interactiveWindow::render
(
    const std::vector<std::vector<node>>& nodes
    // float vecMagMax
)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    
    // std::cout << gridHeight_ << ", " << gridWidth_ << "\n";

    for (int iy = 0; iy < gridHeight_ - 1;  iy++) {
        for (int ix = 0; ix < gridWidth_ - 1;  ix++) {
            // Compute the velocity magnitude

            valueY_ = nodes[iy][ix].Uy();
            valueX_ = nodes[iy][ix].Ux(); //[iy][ix];
            valueMag_ = sqrt(valueX_ * valueX_ + valueY_ * valueY_);

            // Normalize velocity for Jet colormap  
            // valueMagBase_ = (valueMagBase_ < vecMagMax) * vecMagMax + (valueMagBase_ >= vecMagMax) * valueMagBase_;
            float valueNormalized_ = valueMag_ / valueMagBase_;

            // Get the corresponding Jet colormap color
            std::vector<float> color = getJetColor(valueNormalized_);
            // std::cout << valueMag_ << "\n";
            // std::cout << color[0] << ", " << color[1] << ", " << color[2] << "\n";

            // Set color using the Jet colormap
            glColor3f(color[0], color[1], color[2]);
            // glColor3f(225, 225, 225);

            // Draw each grid cell as a square
            glVertex2f(ix, iy);
            glVertex2f(ix, iy + 1);
            glVertex2f(ix + 1, iy + 1);
            glVertex2f(ix + 1, iy);
        }
    }
    
    glEnd();
    SDL_GL_SwapWindow(window_);
}



std::tuple<int, int, float, float> interactiveWindow::fetchCursorMove()
{
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    // Convert window mouse coordinates to grid coordinates
    int gridX = (mouseX * gridWidth_) / windowWidth_;
    int gridY = gridHeight_ - (mouseY * gridHeight_) / windowHeight_;  // Flip Y coordinate for OpenGL grid

    // Get mouse velocity (if mouse is pressed, add velocity to the field)
    static int prevMouseX = gridX, prevMouseY = gridY;
    float dx = .0;
    float dy = .0;

    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        // dx = 0.2;
        // dy = 0.0;
        dx = ((gridX - prevMouseX) * 0.2f);
        dy = ((gridY - prevMouseY) * 0.2f);
    }

    std::tuple<int, int, float, float> ans(gridX, gridY, dx, dy);

    prevMouseX = gridX;
    prevMouseY = gridY;

    // std::cout << " dx: " << dx << " dy: " << dy << std::endl;

    return ans;
}