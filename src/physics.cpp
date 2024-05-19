//
//  physics.cpp
//  VerletIntegration
//
//  Created by Nicholas Moy on 3/31/24.
//

#include "physics.hpp"

using namespace glm;

Sphere::Sphere(vec2 pos)
{
    current_pos = pos;
    previous_pos = pos + VELOCITY_COEFF * vec2(-0.1, 0.0);
}

/*
 * Update position of current sphere one step by dt milliseconds.
 */
void Sphere::updatePosition(float dt)
{
    // Verlet integration step
    vec2 velocity = current_pos - previous_pos;
    vec2 next_pos = current_pos + velocity + (acceleration * dt * dt);

    previous_pos = current_pos;
    current_pos = next_pos;
    //std::cout << current_pos.x << ", " << current_pos.y << "\n";
}

/*
 * Perform one full verlet integration forward dt milliseconds with
 * num_steps intermediate steps.
 */
void Sphere::fullVerletStep(float dt)
{
    float num_steps = NUM_STEPS;
    float intermediate_dt = dt / num_steps;
    while(num_steps > 0)
    {
        updatePosition(intermediate_dt);
        num_steps--;
    }
}

/*
 * Check if the current ray has collided with a sphere 
 */
bool Ray::collideWithSphere(Sphere sphere)
{
    // Write down the quadratic formula thing here
    return true;
}

/*
 * Update positions of all circles on screen
 */
void Physics::update(float dt)
{
    std::vector<Sphere*>::iterator iter;
    float num_steps = NUM_STEPS;
    dt = dt/num_steps;

    // Loop for NUM_STEPS
    while(num_steps > 0)
    {
        // Update position of each sphere
        for(iter = objects.begin(); iter != objects.end(); )
        {
            Sphere *sphere = *iter;
            sphere->updatePosition(dt);
            
            // Resolve collision with stage
            resolveCollisionWithStage(sphere);
            
            // Delete circles out of bounds
            if(!isOutofBounds(sphere))
            {
                iter = objects.erase(iter);
                delete sphere;
            }
            else
            {
                ++iter;
            }
        }
        
        // Resolve collisions
        resolveCollisions();
        
        --num_steps;
    }
}

/*
 * Check if sphere is out of bounds. I give some leeway off screen by about 1.0f.
 */
bool Physics::isOutofBounds(Sphere *sphere)
{
    vec2 pos = sphere->current_pos;
    return (-2.0f < pos.x && pos.x < 2.0f)
        && (-2.0f < pos.y && pos.y < 2.0f);
}

void Physics::resolveCollisionsWithStage()
{
    for(Sphere *sphere : objects)
    {
        resolveCollisionWithStage(sphere);
    }
}

void Physics::resolveCollisionWithStage(Sphere *sphere)
{
    float norm = length(sphere->current_pos);
    if (norm + RADIUS > 1.0f)
    {
        sphere->current_pos = sphere->current_pos * ((1.0f - RADIUS)/norm);
    }
}

/*
 * Just uses O(n^2) check
 */
void Physics::resolveCollisions()
{
    std::vector<Sphere*>::iterator iter_1;
    std::vector<Sphere*>::iterator iter_2;
    for(iter_1 = objects.begin(); iter_1 != objects.end(); )
    {
        Sphere *sphere_1 = *iter_1;
        for(iter_2 = std::next(iter_1, 1); iter_2 != objects.end(); )
        {
            Sphere *sphere_2 = *iter_2;
            
            vec2 pos_1 = sphere_1->current_pos;
            vec2 pos_2 = sphere_2->current_pos;
            
            if(length(pos_2 - pos_1) < 2 * RADIUS)
            {
                vec2 normal = pos_2 - pos_1;
                float norm = length(normal);
                float overlap = RADIUS - norm/2;
                
                sphere_1->current_pos = pos_1 - normal * (overlap/norm);
                sphere_2->current_pos = pos_2 + normal * (overlap/norm);
            }
            
            iter_2++;
        }
        
        iter_1++;
    }
}

Physics::~Physics()
{
    for(Sphere *sphere : objects)
    {
        delete sphere;
    }
}
