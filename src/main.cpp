//
//  main.cpp
//  Raytrace
//
//  Created by Nicholas Moy on 5/15/24.
//

#define GL_SILENCE_DEPRECATION


#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "physics.hpp"
#include "renderer.hpp"
#include "window.hpp"


#define FPS 60
#define FRAME_DELTA_TIME_MIL_SEC 1000.0f/FPS   // Fixed frametime update based on FPS
#define GAME_DELTA_TIME 1.0f/60          // Fixed Game logic delta time


int main(int argc, char *argv[])
{
    
    Window window = Window();
    Renderer renderer = Renderer();
    Physics physics = Physics();

    window.addRenderer(&renderer);
    renderer.addPhysics(&physics);
    
    glm::vec4 Light_Gray = glm::vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0);
    glClearColor(Light_Gray.x, Light_Gray.y, Light_Gray.z, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);
    glClear(GL_COLOR_BUFFER_BIT);

    std::cout << "Game started!";

    // Main loop objects
    SDL_Event e;
    bool running = true;
    unsigned int a = SDL_GetTicks();            // Real time frame update guard
    unsigned int b = SDL_GetTicks();            // Game logic time guard
    unsigned int lastRender = SDL_GetTicks();

    // Main program loop
    while(running)
    {
        // Cap Framerate
        a = SDL_GetTicks();

        if(a < b)
        {
            SDL_Delay(1);
        }
        else
        {
            /* Game Loop */
            while(a > b)
            {
                // Poll events
                window.pollIO(e, &running);
                
                /* Game physics */
                // physics.update(GAME_DELTA_TIME);
                
                b += FRAME_DELTA_TIME_MIL_SEC;
            }
            
            // Render
            renderer.render();
            
            // Update screen
            SDL_GL_SwapWindow(window);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }

    // Clean up SDL Window
    window.quit();
    return 0;
}
