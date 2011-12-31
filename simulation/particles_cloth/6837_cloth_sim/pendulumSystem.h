#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <GL/glut.h>

#include "particleSystem.h"
#include "integrator.h"
#include "spring.h"

extern float SPRING_CONSTANT;
extern float SPRING_LENGTH;
extern float GRAVITY;
extern float DRAG;
extern float MASS;

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(INTEGRATOR_TYPE integrator_type, int numParticles);
        PendulumSystem(); //dummy constructor
        
        vector<Spring> springs;
        vector<vector<Spring*> > springs_of_particles;
        vector<int> stationary_particles;

	vector<Vector3f> evalF(const vector<Vector3f> &state);
	
	void draw();
        
        void initializeParticlesToSprings();
        
        void shake_state();

};


#endif
