#include <iostream>
#include "pendulumSystem.h"
#include <assert.h>

float SPRING_CONSTANT = 55.00;
float SPRING_LENGTH = .50;
float GRAVITY = -9.8;
float DRAG = 0.47;
float MASS = .10;
float SHAKE_AMT = .03;
float SHAKE_MULTIPLIER = 1.0;

PendulumSystem::PendulumSystem():ParticleSystem()
{
    shaking = false;
    wireframe = false;
    wind = false;
} // Dummy constructor

PendulumSystem::PendulumSystem(INTEGRATOR_TYPE integrator_type, int num_particles):ParticleSystem(integrator_type, num_particles)
{
    springs_of_particles.resize(num_particles); // sets length of vector to be number particles, pads with empty vector<Spring*>
    vector<Vector3f> init_state;
    stationary_particles.push_back(0);
    
    shaking = false;
    wireframe = false;
    wind = false;

    // For this pendulum system just did this by hand rather than with for loop. Chain has for loop :)
    init_state.push_back(Vector3f(1, 0, 0)); // position of first particle
    init_state.push_back(Vector3f(0, 0, 0)); // velocity of first particle
    init_state.push_back(Vector3f(2, 0, 0)); // position of second, swinging particle
    init_state.push_back(Vector3f(0, 0, 0)); // velocity of second, swinging particle
     
    // We only need to create one spring
    Spring s = Spring(SPRING_CONSTANT, SPRING_LENGTH, 1, 0);
    springs.push_back(s);

    initializeParticlesToSprings(); 
    assert (springs_of_particles.size() == (unsigned) num_particles);
    setState(init_state);
    
}

vector<Vector3f> PendulumSystem::evalF(const vector<Vector3f> &state)
{
	vector<Vector3f> f;

        Vector3f gravityForce = MASS * Vector3f(0, GRAVITY, 0);

        for (int i = 0; i < num_particles; i++)
        {
            // Get velocity from the state for particle i and push back on derivative
            Vector3f velocity = PositionFinder::getVelocity(state, i);
            f.push_back(velocity);
            
            //viscous drag depends on the drag constant k=-.47 (wikipedia for sphere in air) and velocity
            Vector3f viscousForce = -1.0*DRAG * velocity;
            assert(springs_of_particles.size() == (unsigned) num_particles);
            
            //spring forces depend on the connected particles
            Vector3f net_spring_force = Spring::getNetForce(i, springs_of_particles, state);
            
            Vector3f wind_force = Vector3f(0,0,0);
            //Make wind
            if (wind)
            {
                wind_force = Vector3f(0, 2.0*(float)rand()/(float)RAND_MAX, 0);
                cout << "wind force = " << wind_force[0] << "," << wind_force[1] << "," << wind_force[2] << endl;
            }
             
            Vector3f particle_net_force = gravityForce + viscousForce + net_spring_force + wind_force;
            for (size_t k = 0; k < stationary_particles.size(); k++) // first particle we want stationary
            {
                if (stationary_particles[k] == i)
                {
                   particle_net_force = Vector3f(0,0,0);
                }
            }          
            f.push_back(particle_net_force);     
        }   
	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
      for (int i = 0; i < num_particles; i++) {      
	  Vector3f pos = PositionFinder::getPosition(getState(), i);
          if (!wireframe)
          {
              glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);   
		glPopMatrix();
          }              
	}
        Spring::drawSprings(getState(), springs);
        
        
        
}

void PendulumSystem::initializeParticlesToSprings()
{
    springs_of_particles.resize(num_particles); // resize: sets length of vector to be number particles, pads with empty vector<Spring*>
    
    vector<Spring*> from_particle_springs;
    vector<Spring*> to_particle_springs;
    for (size_t i = 0; i < springs.size(); i++)
    {
        Spring& s = springs[i]; // & makes it a reference variable
        
        vector<Spring*>& from_particle_springs = springs_of_particles[s.from_index];
        vector<Spring*>& to_particle_springs = springs_of_particles[s.to_index];
        
        from_particle_springs.push_back(&s); 
        to_particle_springs.push_back(&s);
    }
    assert (springs_of_particles.size() == (unsigned) num_particles); //invariant
}

/*
 * Makes the cloth move back and forth so you can see the system reacting
 */
void PendulumSystem::shake_state()
{
    vector<Vector3f> new_state = getState();
    for (size_t i = 0; i < stationary_particles.size(); i++)
    {
        int particle_index = stationary_particles[i];
        Vector3f pos = PositionFinder::getPosition(getState(), particle_index);
        
        if (pos[2] > 2.0 ) {
           
            SHAKE_MULTIPLIER = -1.0;
        }
        if (pos[2] < -2.0) {
            
            SHAKE_MULTIPLIER = 1.0;
        }    
        pos[2] = pos[2] + SHAKE_MULTIPLIER*SHAKE_AMT;    
        new_state[particle_index*2] = pos;
    }
    setState(new_state);
}


  