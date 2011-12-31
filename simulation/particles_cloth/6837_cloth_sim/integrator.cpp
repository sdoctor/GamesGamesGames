#include <iostream>
#include "integrator.h"
#include "pendulumSystem.h"
#include <assert.h>

using namespace std;

/*
 * According to X(t+h) = X + h*f(X,t)
 */
vector<Vector3f> Integrator::evalEuler(ParticleSystem* system, float h)
{
  vector<Vector3f> cur_state = system->getState();
  vector<Vector3f> f = system->evalF(cur_state); // the derivative of the state

  vector<Vector3f> new_state; // updated based on the derivative
  
  for (unsigned int i = 0; i < cur_state.size(); i++)
    {
      Vector3f pos = cur_state[i];
      Vector3f f0 = f[i];
      Vector3f new_pos = pos + h*f0;
      new_state.push_back(new_pos);
    }
   return new_state;
}

/*
 *  According to X(t + h) = X + (h/2) * (f0 + f1)
 *  where f0 = f(X,t)
 *  and f1 = f(X+h, t+h)
 */
vector<Vector3f> Integrator::evalTrapezoid(ParticleSystem* system, float h)
{
    vector<Vector3f> cur_state = system->getState();
    vector<Vector3f> f = system->evalF(cur_state);
    vector<Vector3f> interim_vec; // used in the step to get the second deriv
    
    for (unsigned int i = 0; i < cur_state.size(); i++)
	{
	  interim_vec.push_back(cur_state[i] + h*f[i]);
	}

    vector<Vector3f> f1 = system->evalF(interim_vec);
    
    vector<Vector3f> new_state;
    for (unsigned int i = 0; i < cur_state.size(); i++)
    {
        Vector3f pos = cur_state[i];
        Vector3f f0 = f[i];
        Vector3f new_pos = pos + (h*0.5)*(f0 + f1[i]);
        new_state.push_back(new_pos);
    }
  return new_state; 
}


