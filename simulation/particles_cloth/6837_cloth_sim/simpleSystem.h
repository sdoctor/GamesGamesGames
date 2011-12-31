#ifndef SIMPLESYSTEM_H
#define SIMPLESYSTEM_H

#include <GL/glut.h>
#include <vecmath.h>
#include <vector>

#include "particleSystem.h"
#include "integrator.h"

using namespace std;

class SimpleSystem: public ParticleSystem
{
public:
  
        SimpleSystem(INTEGRATOR_TYPE integrator_type, int numParticles);

	vector<Vector3f> evalF(const vector<Vector3f> &state);
	
	void draw();
        
        void shake_state();
	
};

#endif
