#include <iostream>
#include "particleSystem.h"
#include "integrator.h"
#include <cstdlib>

using namespace std;

ParticleSystem::ParticleSystem()
{} // Dummy constructor for semi-hacky inheritance

ParticleSystem::ParticleSystem(INTEGRATOR_TYPE integrator_type, int numParticles)
{
  this->num_particles = numParticles;
  this->integrator_type = integrator_type;
}

void ParticleSystem::stepSystem(float h)
{
  INTEGRATOR_TYPE method = integrator_type;
  vector<Vector3f> next_state;
  
  if (shaking)
  {
      shake_state();
  }
  if (method == EULER)
    {
     next_state = Integrator::evalEuler(this, h);
    }
  else // method == TRAPEZOID
    {
      next_state = Integrator::evalTrapezoid(this, h);	
    }

  setState(next_state);
}

vector<Vector3f> ParticleSystem::getState()
{
  return cur_state;
}

void ParticleSystem::setState(const vector<Vector3f> &new_state)
{
  cur_state = new_state;
}

void ParticleSystem::shake()
{
    if (shaking)
    {
        shaking = false;
    }
    else
    {
        shaking = true;
    }
}

void ParticleSystem::toggle_wireframe()
{
    if (wireframe)
    {
        wireframe = false;
    }
    else
    {
        wireframe = true;
    }
}

void ParticleSystem::toggle_wind()
{
    if (wind)
    {
        wind = false;
    }
    else
    {
        srand((unsigned)time(0)); // seed the random number generator - only has to be done once
        wind = true;
    }
}