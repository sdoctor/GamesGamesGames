/* 
 * File:   BoundingSphere.h
 * Author: drakeley
 *
 * Created on December 3, 2011, 12:00 PM
 */



#ifndef BOUNDINGSPHERE_H
#define	BOUNDINGSPHERE_H

#include "Sphere.h"

class BoundingSphere : public Sphere {
  
public:
    BoundingSphere(Vector3f center, float r, Object3D *obj, Material *m): Sphere(center, r, m){
	object = obj;
    };
    ~BoundingSphere() {};
    bool intersect(const Ray &r, Hit &h, float tMin);
    

private: 
    Object3D *object;

};




#endif	/* BOUNDINGSPHERE_H */

