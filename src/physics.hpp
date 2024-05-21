//
//  physics.hpp
//  VerletIntegration
//
//  Created by Nicholas Moy on 3/31/24.
//

#ifndef physics_hpp
#define physics_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <vector>

#define RADIUS 0.1f
#define GRAVITY -1.0f*5
#define NUM_STEPS 1
#define VELOCITY_COEFF 1.0f/2
#define BOUNCINESS 1.0f


class Sphere
{

public:
    Sphere(glm::vec2 _current_pos);


    glm::vec4 color;

    // Old Verlet Stuff
    glm::vec2 current_pos;
    glm::vec2 previous_pos;
    glm::vec2 acceleration = glm::vec2(0.0, GRAVITY);
    void updatePosition(float dt);
    void fullVerletStep(float dt);
};


class Ray
{

public:
    Ray(glm::vec3 _origin, glm::vec3 _direction, glm::vec4 _color);

    glm::vec3 origin;
    glm::vec3 direction;
    glm::vec4 color;

    // glm::vec4 colorMultiply(glm::vec4 color2);
    bool collideWithSphere(Sphere sphere);
};


class Physics
{
    // Old Verlet Stuff
    void resolveCollisionWithStage(Sphere *sphere);
    bool isOutofBounds(Sphere *sphere);
    
public:
    ~Physics();
    std::vector<Sphere*> objects;
    void resolveCollisionsWithStage();
    void resolveCollisions();

    // Old Verlet Stuff
    void update(float dt);
};


#endif /* physics_hpp */
