/* 
 * File:   multiParticleChainSystem.h
 * Author: drakeley
 *
 * Created on October 30, 2011, 10:39 PM
 */

#ifndef CHAINSYSTEM_H
#define CHAINSYSTEM_H

#include <vector>
#include <vecmath.h>
#include "pendulumSystem.h"
#include "spring.h"

using namespace std;


class Chain : public PendulumSystem
{
public:
    Chain(INTEGRATOR_TYPE integrator_type, int num_particles);
    
};


#endif	/* CHAINSYSTEM_H */

