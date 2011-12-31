#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>

using namespace std;


enum INTEGRATOR_TYPE {EULER, TRAPEZOID};


class ParticleSystem
{
public:

        ParticleSystem();
        ParticleSystem(INTEGRATOR_TYPE integrator_type, int numParticles);
	int num_particles;
        INTEGRATOR_TYPE integrator_type;
	vector<Vector3f> cur_state;
        bool shaking;
        bool wireframe;
        bool wind;
        
	// step the system according to your integrator function
	virtual void stepSystem(float h);
	
	// for a given state, evaluate f(X,t)
	virtual vector<Vector3f> evalF(const vector<Vector3f> &state) = 0;
	
	// getter method for the system's state
	vector<Vector3f> getState();
	
	// setter method for the system's state
	void setState(const vector<Vector3f> &newState);
	
	virtual void draw() = 0;
        
        
        void shake();
        void toggle_wireframe();
        void toggle_wind();
        
        virtual void shake_state() = 0;

	
protected:

};

#endif
