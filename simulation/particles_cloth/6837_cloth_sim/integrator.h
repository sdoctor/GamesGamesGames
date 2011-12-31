#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "vecmath.h"
#include <vector>
#include "particleSystem.h"

using namespace std;

class Integrator
{
 public: 
  static vector<Vector3f> evalEuler(ParticleSystem* system, float h);

  static vector<Vector3f> evalTrapezoid(ParticleSystem* system, float h);

};

#endif
