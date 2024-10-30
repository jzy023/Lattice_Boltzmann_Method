#include "lidDriven.hpp"
#include "interactiveWindow.hpp"


int main()
{
    int itr = 0;
    lidDriven domain(66, 0.2, 1000);
    std::tuple<int, int, float, float> cursorVel;


    SDL_Event event;
    interactiveWindow window("lid-driven Umag profile", domain.getNx());

    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;  // Exit the loop when the window is closed
            }
        }

        std::cout << ">>> Iteration: " << itr;

        // update domain
        domain.update();
        domain.macroscopic();

        // fetch domain vels
        std::vector<std::vector<float>> ux = domain.getUx();
        std::vector<std::vector<float>> uy = domain.getUy();
        
        // render
        window.render(ux, uy, domain.getUmagMax());

        itr++;
        std::cout << std::endl;

        // SDL_Delay(15);
    }

    return 0;
}
