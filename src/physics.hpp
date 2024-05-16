//
//  physics.hpp
//  VerletIntegration
//
//  Created by Nicholas Moy on 3/31/24.
//

#ifndef physics_hpp
#define physics_hpp

#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

#define RADIUS 0.1f
#define GRAVITY -1.0f*5
#define NUM_STEPS 1
#define VELOCITY_COEFF 1.0f/2
#define BOUNCINESS 1.0f

#endif /* physics_hpp */


class Circle
{
    glm::vec2 velc_ghost_pos;
    
    
public:
    Circle(glm::vec2 _current_pos);
    void update_position(float dt);
    glm::vec2 current_pos;
    glm::vec2 previous_pos;
    glm::vec2 acceleration = glm::vec2(0.0, GRAVITY);
    void full_verlet_step(float dt);
};


class Physics
{
    void resolveCollisionWithStage(Circle *circle);
    bool isOutofBounds(Circle *circle);
    
public:
    ~Physics();
    std::vector<Circle*> objects;
    void update(float dt);
    bool setPosition(Circle *circle);
    void resolveCollisionsWithStage();
    void resolveCollisions();
};
