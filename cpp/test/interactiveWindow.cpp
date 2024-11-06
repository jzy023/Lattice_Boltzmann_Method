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


// // Initialize the GIF file
// bool interactiveWindow::initializeGif(const char* filename) {
//     gifColorMap_.ColorCount = 256;       // Set to 256 for an 8-bit color palette
//     gifColorMap_.BitsPerPixel = 8;       // 8 bits per pixel (256 colors)
//     gifColorMap_.SortFlag = false;
//     gifColorMap_.Colors = new GifColorType[gifColorMap_.ColorCount]; // Allocate color map

//     // Populate color map with Jet colormap
//     for (int i = 0; i < gifColorMap_.ColorCount; ++i) {
//         float normalizedValue = static_cast<float>(i) / 255.0f;
//         std::vector<float> color = getJetColor(normalizedValue);
        
//         gifColorMap_.Colors[i].Red = static_cast<GifByteType>(color[0] * 255);
//         gifColorMap_.Colors[i].Green = static_cast<GifByteType>(color[1] * 255);
//         gifColorMap_.Colors[i].Blue = static_cast<GifByteType>(color[2] * 255);
//     }

//     // Allocate pixel buffer once for the grid size (66x66)
//     pixels_ = new unsigned char[gridWidth_ * gridHeight_ * 3]; // RGB

//     gifFileName_ = filename; // Store filename
//     gifFile_ = EGifOpenFileName(gifFileName_, false, NULL); // Open GIF for writing
//     if (!gifFile_) {
//         SDL_Log("Error opening GIF file: %s", gifFileName_);
//         delete[] pixels_; // Clean up allocated memory
//         return false;
//     }

//     // Set the GIF screen descriptor
//     if (EGifPutScreenDesc(gifFile_, gridWidth_, gridHeight_, 256, 0, &gifColorMap_) == GIF_ERROR) {
//         SDL_Log("Error setting GIF screen descriptor");
//         EGifCloseFile(gifFile_, NULL); // Close on failure
//         gifFile_ = nullptr; // Reset pointer
//         delete[] pixels_; // Clean up allocated memory
//         return false;
//     }

//     frameCount_ = 0; // Reset frame count
//     return true;
// }

// // Save a frame to the GIF
// void interactiveWindow::saveFrameToGif() {
//     // Read pixels from the OpenGL buffer for the grid size
//     glReadPixels(0, 0, gridWidth_, gridHeight_, GL_RGB, GL_UNSIGNED_BYTE, pixels_);

//     // std::cout << "\nR " << static_cast<int>(pixels_[66*10 - 3]) 
//     //           << "; G " << static_cast<int>(pixels_[66*10 - 2]) 
//     //           << "; B " << static_cast<int>(pixels_[66*10 - 1]) << std::endl;

//     if (!gifFile_) {
//         SDL_Log("GIF file is not open. Cannot save frame.");
//         return;
//     }

//     // Flip pixels vertically (if needed)
//     // unsigned char* flippedPixels = new unsigned char[gridWidth_ * gridHeight_ * 3]; // 66x66 pixels
//     // for (int y = 0; y < gridHeight_; y++) {
//     //     for (int x = 0; x < gridWidth_; x++) {
//     //         int srcIndex = (x + (gridHeight_ - 1 - y) * gridWidth_) * 3; // Source index for flipping
//     //         int dstIndex = (x + y * gridWidth_) * 3; // Destination index
//     //         flippedPixels[dstIndex] = pixels_[srcIndex];        // R
//     //         flippedPixels[dstIndex + 1] = pixels_[srcIndex + 1]; // G
//     //         flippedPixels[dstIndex + 2] = pixels_[srcIndex + 2]; // B
//     //     }
//     // }

//     // Write image descriptor for the current frame
//     if (EGifPutImageDesc(gifFile_, 0, 0, gridWidth_, gridHeight_, false, &gifColorMap_) == GIF_ERROR) {
//         std::cout << " ERROR! " << gifFile_->Error << std::endl;
//         // delete[] flippedPixels; // Clean up
//         return;
//     }

//     // Write pixel data to GIF
//     if (EGifPutLine(gifFile_, pixels_, gridWidth_ * gridHeight_ * 3) == GIF_ERROR) {
//         std::cout << " ERROR! " << gifFile_->Error << std::endl;
//         return;
//     }

//     // Clean up
//     // delete[] flippedPixels; 
//     frameCount_++;
// }

// // Close the GIF file
// void interactiveWindow::closeGif() {
//     if (gifFile_) {
//         EGifCloseFile(gifFile_, NULL); // Close the GIF file
//         gifFile_ = nullptr;
//     }
//     delete[] gifColorMap_.Colors;
//     delete[] pixels_; // Clean up allocated pixel buffer
// }



// methods
void interactiveWindow::render
(
    const std::vector<std::vector<float>>& vec,
    const float vecMax
)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);

    for (int x = 0; x < gridWidth_ - 1; ++x) {
        for (int y = 0; y < gridHeight_ - 1; ++y) {

            float vecMagBase = vecMax;
            float normalizedValue = vec[x][y] / vecMagBase;

            // Get the corresponding Jet colormap color
            std::vector<float> color = getJetColor(normalizedValue);

            // Set color using the Jet colormap
            glColor3f(color[0], color[1], color[2]);

            // Draw each grid cell as a square
            glVertex2f(y, x);
            glVertex2f(y, x + 1);
            glVertex2f(y + 1, x + 1);
            glVertex2f(y + 1, x);
        }
    }

    glEnd();
    SDL_GL_SwapWindow(window_); 
}


void interactiveWindow::render
(
    const std::vector<std::vector<float>>& vecX, 
    const std::vector<std::vector<float>>& vecY,
    const float vecMagMax
)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);

    float vecMagBase = 0.2;
    for (int x = 0; x < gridWidth_ - 1; ++x) {
        for (int y = 0; y < gridHeight_ - 1; ++y) {
            // Compute the velocity magnitude
            float vecMag = sqrt(vecX[x][y] * vecX[x][y] + vecY[x][y] * vecY[x][y]);

            // Normalize velocity for Jet colormap  
            vecMagBase = (vecMagBase < vecMagMax) * vecMagMax + (vecMagBase >= vecMagMax) * vecMagBase;
            float normalizedValue = vecMag / vecMagBase;

            // Get the corresponding Jet colormap color
            std::vector<float> color = getJetColor(normalizedValue);

            // Set color using the Jet colormap
            glColor3f(color[0], color[1], color[2]);

            // Draw each grid cell as a square
            glVertex2f(y, x);
            glVertex2f(y, x + 1);
            glVertex2f(y + 1, x + 1);
            glVertex2f(y + 1, x);
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