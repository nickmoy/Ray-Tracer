//
//  window.cpp
//  RayTracer
//
//  Created by Nicholas Moy on 4/10/24.
//

#include "window.hpp"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "glm/fwd.hpp"
#include <SDL2/SDL.h>
#include <iostream>

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
        }
        // Only detects the position when the Mouse is Moved
        else if(e.type == SDL_MOUSEMOTION && e.motion.state & SDL_BUTTON_LMASK)
        {
            // Counts pixels starting from bottom left corner. The way it should be you fuckers!
            moved_to_x += e.motion.x;
            moved_to_y += 768 - e.motion.y;
            renderer->rotateCamera(moved_to_x - clicked_at.x, moved_to_y - clicked_at.y);
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
            renderer->doneRotatingCamera();
            // Create new circles when mouse is clicked
            // int pixel_x, pixel_y;
            // SDL_GetMouseState(&pixel_x, &pixel_y);
            // //std::cout << "(x,y) : " << "(" << pixel_x << ", " << pixel_y << ")" << "\n";
            // float x = ((float)pixel_x/768) * 2 - 1;
            // float y = 1 - ((float)pixel_y/768) * 2;
            // 
            // Sphere *sphere = new Sphere(vec2(x, y)); /* Need to change sphere constructor here */
            // renderer->physics->objects.push_back(sphere);
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
