//
//  window.cpp
//  VerletIntegration
//
//  Created by Nicholas Moy on 4/10/24.
//

#include "window.hpp"

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
    
    window = SDL_CreateWindow("Verlet Integration", SDL_WINDOWPOS_CENTERED,
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
 * Left click mouse to create circle on screen
 */
void Window::PollIO(SDL_Event e, bool *running)
{
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
        else if(e.type == SDL_MOUSEBUTTONUP)
        {
            // changeColor((float)(rand() % 100)/100, (float)(rand() % 100)/100, (float)(rand() % 100)/100, window);
            /* Generate center for circle of radius=RADIUS so that it stays on screen */
            //glm::vec3 center = glm::vec3((float)(rand() % 100)/100 * (2 - 2*RADIUS) - (1 - RADIUS), (float)(rand() % 100)/100 * (2 - 2*RADIUS) - (1 - RADIUS), 0.0);
            //renderer->setCircleCenter(center);
            
            // Create new circles when mouse is clicked
            int pixel_x, pixel_y;
            SDL_GetMouseState(&pixel_x, &pixel_y);
            //std::cout << "(x,y) : " << "(" << pixel_x << ", " << pixel_y << ")" << "\n";
            float x = ((float)pixel_x/768) * 2 - 1;
            float y = 1 - ((float)pixel_y/768) * 2;
            
            Circle *circle = new Circle(vec2(x, y));
            renderer->physics->objects.push_back(circle);
        }
    }
}

void Window::addRenderer(Renderer *_renderer)
{
    renderer = _renderer;
}
