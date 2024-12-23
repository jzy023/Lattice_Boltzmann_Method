#include "confinedBox.hpp"
#include "interactiveWindow.hpp"


int main()
{
    int sampleFreq = 100;
    long unsigned itr = 0;
    confinedBox domain(66, 1000);
    std::tuple<int, int, float, float> cursorVel;

    // make dir for output data
    domain.mkdirDataOutput("data");

    SDL_Event event;
    interactiveWindow window("confined box Umag profile", domain.getNx());

    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;  // Exit the loop when the window is closed
            }
        }

        std::cout << ">>> Iteration: " << itr;

        // update domain
        domain.addCursorVel(cursorVel);
        domain.update();
        domain.macroscopic();

        // log data
        if (itr % sampleFreq == 0) { domain.wrtieDataCSV(sampleFreq); }

        // fetch domain vels
        std::vector<std::vector<float>> ux = domain.getUx();
        std::vector<std::vector<float>> uy = domain.getUy();
        
        // render
        window.render(ux, uy, domain.getUmagMax());
        cursorVel = window.fetchCursorMove();

        itr++;
        std::cout << std::endl;

        // SDL_Delay(15);
    }

    return 0;
}
