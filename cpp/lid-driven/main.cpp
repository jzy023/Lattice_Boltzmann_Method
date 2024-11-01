#include "lidDriven.hpp"
#include "interactiveWindow.hpp"


int main()
{
    int sampleFreq = 100;
    long unsigned nItr = 1e4;
    long unsigned itr = 0;

    lidDriven domain(66, 0.2, 1000);
    std::tuple<int, int, float, float> cursorVel;

    // make dir for output data
    domain.mkdirDataOutput("data");
 
    SDL_Event event;
    interactiveWindow window("lid-driven Umag profile", domain.getNx());

    bool running = true;
    while (running && itr <= nItr) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;  // Exit the loop when the window is closed
            }
        }

        std::cout << ">>> Iteration: " << itr;

        // update domain
        domain.update();
        domain.macroscopic();

        // log data
        if (itr % sampleFreq == 0) { domain.wrtieDataCSV(sampleFreq); }
        
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
