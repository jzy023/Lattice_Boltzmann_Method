#include "node.hpp"
#include "domain.hpp"
#include "interactiveWindow.hpp"


int main()
{
    int size = 80;

    domain Domain(size, 0.1, 1000);

    //             yMin    xMin     yMax    xMax
    Domain.setWall(0,      size-1,  size-1, size-1); // East wall
    // Domain.setWall(size-1, 0,       size-1, size-1); // North wall
    Domain.setWall(0,      0,       size-1, 0);      // West wall
    Domain.setWall(0,      0,       0,      size-1); // South wall
//    *Domain.nodes[3][3].setF() = {10, 10, 10, 10, 10, 10, 10, 10};
//    *Domain.nodes[3][3].setF0() = {10, 10, 10, 10, 10, 10, 10, 10};
//    *Domain.nodes[3][3].setF() = {10, 0, 0, 0, 0, 0, 0, 0};
//    *Domain.nodes[3][3].setF0() = {10, 0, 0, 0, 0, 0, 0, 0};

    // Domain.printTypes();


    int itr = 0;
    int nIter = 10;
    SDL_Event event;
    interactiveWindow window("2", Domain.getNx());

    bool running = true;
    // while (running && itr < nIter) {
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;  // Exit the loop when the window is closed
            }
        }

        std::cout << ">>> Iteration: " << itr;

        // // fetch domain vels
        // std::vector<std::vector<float>> ux = Domain.getUx();
        // std::vector<std::vector<float>> uy = Domain.getUy();

        // // render and fetch cursor
        // window.render(ux, uy, Domain.getUmagMax());
        window.render(Domain.nodes);

        // update domain
        // Domain.printUmag();
        Domain.update();
        // TODO !!! rewrite window.render() to take node and map directly (y,x) to pixels instead of taking in 2D vectors
        
        itr++;
        // SDL_Delay(10);
        std::cout << std::endl;
        // Domain.printTypes();
    }
    // Domain.printUmag();



    // int itr = 0;
    // int nIter = 1e3;
    // domain confinedBox(100, 0.0, 1000);


    // SDL_Event event;
    // interactiveWindow window("2", confinedBox.getNx());

    // bool running = true;
    // // while (running && itr < nIter) {
    // while (running) {
    //     while (SDL_PollEvent(&event)) {
    //         if (event.type == SDL_QUIT) {
    //             running = false;  // Exit the loop when the window is closed
    //         }
    //     }
    //     std::cout << ">>> Iteration: " << itr;

    //     // fetch domain vels
    //     std::vector<std::vector<float>> ux = confinedBox.getUx();
    //     std::vector<std::vector<float>> uy = confinedBox.getUy();

    //     // render and fetch cursor
    //     window.render(ux, uy, confinedBox.getUmagMax());

    //     // update domain
    //     confinedBox.update();
    //     confinedBox.macroscopic();
        
    //     itr++;
    //     SDL_Delay(5);
    //     std::cout << std::endl;
    // }


    return 0;

}
