#include "node.hpp"
#include "domain.hpp"
#include "interactiveWindow.hpp"


int main()
{
    int itr = 0;
    int nIter = 1e5;

    int size = 80;
    domain Domain(size, 0.1, 1000);
    //             yMin    xMin     yMax    xMax
    Domain.setBcWall(0,      size-1,  size-1, size-1); // East wall
    Domain.setBcWall(0,      0,       size-1, 0);      // West wall
    Domain.setBcWall(0,      0,       0,      size-1); // South wall

    SDL_Event event;
    interactiveWindow window("2", Domain.Nx());

    auto start = std::chrono::high_resolution_clock::now();
    bool running = true;
    
    // #pragma omp parallel
    while (running && itr < nIter) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;  // Exit the loop when the window is closed
            }
        }

        std::cout << ">>> Iteration: " << itr;

        // // render and fetch cursor
        window.render(Domain.Nodes());

        // update domain
        Domain.update();
        // TODO !!! rewrite window.render() to take node and map directly (y,x) to pixels instead of taking in 2D vectors
        
        itr++;
        std::cout << std::endl;
        
        // SDL_Delay(10);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << std::fixed << std::setprecision(6) 
              << "Time taken to finish simulation: " << duration.count() 
              << " seconds" << std::endl;
    return 0;
}
