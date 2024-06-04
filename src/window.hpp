//
//  window.h
//  RayTracer
//
//  Created by Nicholas Moy on 4/10/24.
//

#ifndef window_hpp
#define window_hpp

#define GL_SILENCE_DEPRECATION

#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_keycode.h"
#include "glm/fwd.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include "renderer.hpp"


class Window
{

private:
    glm::vec2 clicked_at;
    int keys_held[2];
    
public:
    SDL_Window *window;
    SDL_GLContext context;
    Renderer *renderer;
    
    Window();
    operator SDL_Window* () const {return window;}
    void changeColor(float R, float G, float B, float A);
    void pollIO(SDL_Event e, bool *running);
    void addRenderer(Renderer *_renderer);

    // Cleanup
    void quit();
};


#endif /* window_h */
