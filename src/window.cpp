//
//  window.cpp
//  RayTracer
//
//  Created by Nicholas Moy on 4/10/24.
//

#include "window.hpp"
#include "SDL2/SDL_keycode.h"


using namespace glm;


Window::Window()
{
    /*
     * Initialize window
     */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL Error: " << SDL_GetError() << "\n";
    }
    
    window = SDL_CreateWindow("Ray Tracer", SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            768, 768,
                                            SDL_WINDOW_OPENGL);
    if(!window)
    {
        std::cout << "Failed to create window\n";
    }
    
    // Set window attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Create OpenGL context
    context = SDL_GL_CreateContext(window);
    SDL_UpdateWindowSurface(window);
    SDL_GL_SetSwapInterval(1);

    clicked_at = vec2(768.0f/2, 768.0f/2);
    keys_held[0] = -1;
    keys_held[1] = -1;
}

void changeColor(float R, float G, float B, float A)
{
    glClearColor(R, G, B, A);
    glClear(GL_COLOR_BUFFER_BIT);
}

/*
 * Poll IO events
 *
 * Click "q" to quit program
 * Left click mouse to create sphere on screen
 */
void Window::pollIO(SDL_Event e, bool *running)
{
    if(keys_held[0] == SDLK_k)
    {
        renderer->camera.translateCameraLeft();
    }
    else if(keys_held[0] == SDLK_SEMICOLON)
    {
        renderer->camera.translateCameraRight();
    }
    else if(keys_held[0] == SDLK_o)
    {
        renderer->camera.translateCameraFoward();
    }
    else if(keys_held[0] == SDLK_l)
    {
        renderer->camera.translateCameraBack();
    }
    else if(keys_held[0] == SDLK_w)
    {
        renderer->camera.translateCameraUp();
    }
    else if(keys_held[0] == SDLK_s)
    {
        renderer->camera.translateCameraDown();
    }

    float moved_to_x = 0;
    float moved_to_y = 0;
    while(SDL_PollEvent(&e))
    {
        
        if(e.type == SDL_QUIT)
        {
            *running = false;
        }
        else if(e.type == SDL_KEYDOWN)
        {
            std::cout << (char)e.key.keysym.sym;
            std::cout.flush();

            switch(e.key.keysym.sym)
            {
                case SDLK_q:
                    std::cout << "\nYou quit!\n";
                    *running = false;
                    break;
            }

            keys_held[0] = e.key.keysym.sym;
        }
        else if (e.type == SDL_KEYUP) {
            keys_held[0] = -1;
        }
        // Only detects the position when the Mouse is Moved
        else if(e.type == SDL_MOUSEMOTION && e.motion.state & SDL_BUTTON_LMASK)
        {
            // Counts pixels starting from bottom left corner. The way it should be, you fuckers!
            moved_to_x += e.motion.x;
            moved_to_y += 768 - e.motion.y;
            renderer->camera.rotateCamera(moved_to_x - clicked_at.x, moved_to_y - clicked_at.y);

            // std::cout << "Mouse moved to: (" << moved_to_x << ", " << moved_to_y << ")\n";
            // std::cout << "Rotated by: (" << moved_to_x - clicked_at.x << ", " << moved_to_y - clicked_at.y << ")\n";
            // std::cout << "Last mouse click at: (" << clicked_at.x << ", " << clicked_at.y << ")\n";
            // std::cout.flush();
        }
        // Only detects the position when the Mouse is JUST Clicked Down
        else if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            clicked_at.x = e.motion.x;
            clicked_at.y = 768 - e.motion.y;
            // std::cout << "Mouse just got clicked down at: (" << clicked_at.x << ", " << clicked_at.y << ")\n";
        }
        // Only detects the position when Mouse Click Ends
        else if(e.type == SDL_MOUSEBUTTONUP)
        {
            // std::cout << "Mouse clicked ended at: (" << e.motion.x << ", " << 768 - e.motion.y << ")\n";
            // std::cout.flush();
            renderer->camera.doneRotating();
        }
    }
}

void Window::addRenderer(Renderer *_renderer)
{
    renderer = _renderer;
}

void Window::quit() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
