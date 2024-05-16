//
//  window.h
//  VerletIntegration
//
//  Created by Nicholas Moy on 4/10/24.
//

#ifndef window_h
#define window_h

#define GL_SILENCE_DEPRECATION

#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <iostream>
#include <format>

#ifndef renderer_hpp
#include "renderer.hpp"
#endif

#endif /* window_h */


class Window
{
    
public:
    SDL_Window *window;
    SDL_GLContext context;
    Renderer *renderer;
    
    Window();
    operator SDL_Window* () const {return window;}
    void changeColor(float R, float G, float B, float A);
    void PollIO(SDL_Event e, bool *running);
    void addRenderer(Renderer *_renderer);
};
