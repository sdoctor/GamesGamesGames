#include "chainSystem.h"
#include "integrator.h"
#include "pendulumSystem.h"
#include <iostream>
#include <assert.h>

/*
 * A particle system of an arbitrary number of particles linked by single springs
 * Initializes
 */
Chain::Chain(INTEGRATOR_TYPE integrator_type, int num_particles):PendulumSystem()
{
    integrator_type = integrator_type; // Euler or Trapezoid
    num_particles = num_particles;
    
    stationary_particles.push_back(0); // only one pivot point at first particle
    
    // initialize the state based on the number of particles
    vector<Vector3f> init_state;
    for (int i = 0; i < num_particles; i++) 
    {
        // for this system, we care about the position and the velocity
        Vector3f pos = Vector3f(.5*(i+1),0,0); 
        Vector3f velocity = Vector3f(0,0,0);

        init_state.push_back(pos);
        init_state.push_back(velocity);

        // Hook up springs
        if (i > 0 and !Spring::springInSprings(springs, i, i-1)) 
        { 
            springs.push_back(Spring(SPRING_CONSTANT, SPRING_LENGTH, i, i-1));
        }
    }

    initializeParticlesToSprings(); 
    
    assert (springs_of_particles.size() == (unsigned) num_particles);
    
    setState(init_state);  // pass reference or else copies (const?)
}





