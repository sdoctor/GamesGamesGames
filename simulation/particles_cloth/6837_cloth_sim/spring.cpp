#include <iostream>
#include "spring.h"
#include "pendulumSystem.h"
#include <assert.h>

Spring::Spring(float k, float rest_length, int from_index, int to_index)
{
    this->k = k;
    this->rest_length = rest_length;
    this->from_index = from_index;
    this->to_index = to_index;
}

bool Spring::leftEdge(int width, int j)
{
    return (j%width == 0);
}
bool Spring::rightEdge(int width, int j)
{
    return ((j+1)%width == 0);
}
bool Spring::topEdge(int i)
{
    return (i == 0);
}
bool Spring::bottomEdge(int height, int i)
{
    return (i == height-1);
}

Vector3f Spring::getNetForce(int particle_index, const vector<vector<Spring*> > &particle_springs, const vector<Vector3f> &state)
{
    assert ((unsigned) particle_index < particle_springs.size());
    vector<Spring*> springs_for_particle = particle_springs[particle_index];
    Vector3f net_force = Vector3f(0,0,0);
    
    for (unsigned int i = 0; i < springs_for_particle.size(); i++)
    {
        Spring* s = springs_for_particle[i];
        float k = s->k;
        float r = s->rest_length; 
        int from_index = s->from_index;
        int to_index = s->to_index;
         
        Vector3f xi = PositionFinder::getPosition(state, from_index);
        Vector3f xj = PositionFinder::getPosition(state, to_index);
        
        int multiplier = (particle_index == to_index) ? 1 : -1;
        
        Vector3f d = multiplier*(xj - xi);
        float d_mag = d.abs();
        Vector3f d_norm = d.normalized();
        
        Vector3f result_force = -k*(d_mag - r)*d_norm;
        
        net_force += result_force;
        
    }
    return net_force;
}
    
 


void Spring::drawSprings(const vector<Vector3f> &state, const vector<Spring> &springs)
{
    //This springs data structure may not be optimal...since you draw springs multiple times...
    for (unsigned int i = 0; i < springs.size(); i++) // not sure why i had to do +1... 
    {
            Spring s = springs[i];
            assert ((unsigned) s.to_index < state.size()/2);
            Vector3f pos0 = PositionFinder::getPosition(state, s.from_index);
            Vector3f pos1 = PositionFinder::getPosition(state, s.to_index);
            
            glLineWidth(0.5);
            glBegin(GL_LINES);
            glVertex3f(pos0[0], pos0[1], pos0[2]);
            glVertex3f(pos1[0], pos1[1], pos1[2]); 
            glEnd();
    }
}

bool Spring::springInSprings(const vector<Spring> &springs, int particle_1, int particle_2)
{
    for (int i = 0; (unsigned) i < springs.size(); i ++)
    {
        Spring s = springs[i];
        if (s.to_index == particle_1 and s.from_index == particle_2)
        {
            return true;
        }
        if (s.to_index == particle_2 and s.from_index == particle_1)
        {
            return true;
        }
    }
    return false;
}

Vector3f PositionFinder::getPosition(const vector<Vector3f> &state, int particle_index)
{
    return state[2*particle_index];
}

Vector3f PositionFinder::getVelocity(const vector<Vector3f> &state, int particle_index)
{
    return state[2*particle_index+1];
}