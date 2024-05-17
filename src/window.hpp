//
//  window.h
//  VerletIntegration
//
//  Created by Nicholas Moy on 4/10/24.
//

#ifndef window_hpp
#define window_hpp

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>

#include "renderer.hpp"


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

    // Cleanup
    void quit();
};


#endif /* window_h */
