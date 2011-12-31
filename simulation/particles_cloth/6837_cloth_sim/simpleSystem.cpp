
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem(INTEGRATOR_TYPE integrator_type, int num_particles) : ParticleSystem(integrator_type, num_particles)
{
    this->integrator_type = integrator_type;
    vector<Vector3f> init_state;
    Vector3f init_pos = Vector3f(1,1,1);
    init_state.push_back(init_pos);
    cur_state = init_state;
}

// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(const vector<Vector3f> &state)
{
    vector<Vector3f> f;

    // *Note* State will only have one entry because one particle
    // f(X, t) = (-y, x, 0)
    f.push_back(Vector3f(-state[0][1], state[0][0], 0));

    return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
    Vector3f pos = SimpleSystem::getState()[0];
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2] );
    glutSolidSphere(0.075f,10.0f,10.0f);
    glPopMatrix();
}

void SimpleSystem::shake_state(){}

