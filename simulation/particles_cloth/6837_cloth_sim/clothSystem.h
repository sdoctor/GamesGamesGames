/* 
 * File:   clothSystem.h
 * Author: drakeley
 *
 * Created on October 31, 2011, 1:14 AM
 */

#ifndef CLOTHSYSTEM_H
#define	CLOTHSYSTEM_H

#include <vector>
#include <vecmath.h>
#include "pendulumSystem.h"
#include "spring.h"

using namespace std;


class Cloth : public PendulumSystem
{
public:
    Cloth(INTEGRATOR_TYPE integrator_type, int width, int height);
    
    int width;
    int height;
	  
    int indexOf(int i, int j);
    int jOf(int particle_index);
    int iOf(int particle_index, int j);
    
    void attachSprings(int from_index, int to_index);
//    void draw();
    
    
};

#endif	/* CLOTHSYSTEM_H */

