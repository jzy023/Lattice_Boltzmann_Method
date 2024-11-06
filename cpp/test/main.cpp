#include "node.hpp"
#include "domain.hpp"
#include "interactiveWindow.hpp"


int main()
{

    domain Domain(5, 0.0, 1000);

    for (int ix = 0; ix < 10; ix++)
    {
        Domain.nodes[0][ix].setBound();
    }

    Domain.printTypes();

    // std::cout << Domain.nodes[0][1].getType();



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
