//
//  physics.cpp
//  VerletIntegration
//
//  Created by Nicholas Moy on 3/31/24.
//

#include "physics.hpp"

using namespace glm;

Circle::Circle(vec2 pos)
{
    current_pos = pos;
    previous_pos = pos + VELOCITY_COEFF * vec2(-0.1, 0.0);
}

/*
 * Update position of current circle one step by dt milliseconds.
 */
void Circle::update_position(float dt)
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
void Circle::full_verlet_step(float dt)
{
    float num_steps = NUM_STEPS;
    float intermediate_dt = dt / num_steps;
    while(num_steps > 0)
    {
        update_position(intermediate_dt);
        num_steps--;
    }
}

/*
 * Update positions of all circles on screen
 */
void Physics::update(float dt)
{
    std::vector<Circle*>::iterator iter;
    float num_steps = NUM_STEPS;
    dt = dt/num_steps;

    // Loop for NUM_STEPS
    while(num_steps > 0)
    {
        // Update position of each circle
        for(iter = objects.begin(); iter != objects.end(); )
        {
            Circle *circle = *iter;
            circle->update_position(dt);
            
            // Resolve collision with stage
            resolveCollisionWithStage(circle);
            
            // Delete circles out of bounds
            if(!isOutofBounds(circle))
            {
                iter = objects.erase(iter);
                delete circle;
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
 * Check if circle is out of bounds. I give some leeway off screen by about 1.0f.
 */
bool Physics::isOutofBounds(Circle *circle)
{
    vec2 pos = circle->current_pos;
    return (-2.0f < pos.x && pos.x < 2.0f)
        && (-2.0f < pos.y && pos.y < 2.0f);
}

void Physics::resolveCollisionsWithStage()
{
    for(Circle *circle : objects)
    {
        resolveCollisionWithStage(circle);
    }
}

void Physics::resolveCollisionWithStage(Circle *circle)
{
    float norm = length(circle->current_pos);
    if (norm + RADIUS > 1.0f)
    {
        circle->current_pos = circle->current_pos * ((1.0f - RADIUS)/norm);
    }
}

/*
 * Just uses O(n^2) check
 */
void Physics::resolveCollisions()
{
    std::vector<Circle*>::iterator iter_1;
    std::vector<Circle*>::iterator iter_2;
    for(iter_1 = objects.begin(); iter_1 != objects.end(); )
    {
        Circle *circle_1 = *iter_1;
        for(iter_2 = std::next(iter_1, 1); iter_2 != objects.end(); )
        {
            Circle *circle_2 = *iter_2;
            
            vec2 pos_1 = circle_1->current_pos;
            vec2 pos_2 = circle_2->current_pos;
            
            if(length(pos_2 - pos_1) < 2 * RADIUS)
            {
                vec2 normal = pos_2 - pos_1;
                float norm = length(normal);
                float overlap = RADIUS - norm/2;
                
                circle_1->current_pos = pos_1 - normal * (overlap/norm);
                circle_2->current_pos = pos_2 + normal * (overlap/norm);
            }
            
            iter_2++;
        }
        
        iter_1++;
    }
}

Physics::~Physics()
{
    for(Circle *circle : objects)
    {
        delete circle;
    }
}
