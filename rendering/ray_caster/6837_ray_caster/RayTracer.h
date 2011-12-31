/* 
 * File:   RayTracer.h
 * Author: drakeley
 *
 * Created on December 3, 2011, 12:54 PM
 */

#ifndef RAYTRACER_H
#define	RAYTRACER_H

#include <vecmath.h>
#include <vector>
#include "Ray.h"
#include "Hit.h"
#include "SceneParser.h"

using namespace std;


extern float EPSILON;

class RayTracer {
public:
    RayTracer(){};
    
    RayTracer(SceneParser *sp, int mb, float wc, bool sh);
    
    virtual ~RayTracer(){};
    
    Vector3f traceRay(Ray &ray, float tMin, int bounces, float weight,
    Hit &hit) const;
    
    static Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming );
    
protected:
    
    SceneParser* scene_parser;
    int max_bounces;
    float cutoff_weight;
    bool shadows;
    bool refraction;
 };

#endif	/* RAYTRACER_H */

