/* 
 * File:   spring.h
 * Author: drakeley
 *
 * Created on October 30, 2011, 10:43 PM
 */

#ifndef SPRING_H
#define	SPRING_H

#include <vector>
#include <vecmath.h>


using namespace std;

class Spring
{
  public:
        Spring(float k, float rest_length, int from_index, int to_index);  
        float k;
        float rest_length;
        int from_index;
        int to_index;
        
        Vector3f getForce(Vector3f from_pos, Vector3f to_pos);
        
        static bool leftEdge(int width, int i);
        static bool rightEdge(int width, int i);
        static bool topEdge(int j);
        static bool bottomEdge(int height, int j);

        static Vector3f getNetForce(int particle_index, const vector<vector<Spring*> > &particle_springs, const vector<Vector3f> &state);
        static void drawSprings(const vector<Vector3f> &state, const vector<Spring> &springs);
        
        static vector<Spring> getSpringsForParticle(const vector<Spring> &springs, int particle_index);
        
        static bool springInSprings(const vector<Spring> &springs, int particle_1, int particle_2);
};

class PositionFinder
{
public: 
    
    static Vector3f getPosition(const vector<Vector3f> &state, int particle_index);
    static Vector3f getVelocity(const vector<Vector3f> &state, int particle_index);
    
};

#endif	/* SPRING_H */

