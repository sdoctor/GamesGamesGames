#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;

class Sphere: public Object3D
{
public:
	Sphere(){ 
		//unit ball at the center
		center = Vector3f(0.0 , 0.0 , 0.0);
		radius = 1.0f;
	}

	Sphere( Vector3f center , float radius , Material* material ):Object3D(material){
		this->center = center ; 
		this->radius = radius ; 
	}
	

	~Sphere(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		// Locate intersection point ( 2 pts )
		Vector3f rayOrigin = r.getOrigin(); //Ray origin in the world coordinate
		Vector3f dir = r.getDirection(); 

		Vector3f origin = rayOrigin - center;      //Ray origin in the sphere coordinate
		
		float a = dir.absSquared();
		float b = 2*Vector3f::dot( dir , origin );
		float c = origin.absSquared() - radius*radius ;	
		
		// no intersection
		if( b*b - 4*a*c < 0 ){ return false ;}

		float d = sqrt( b*b - 4*a*c );
		
		float tplus = (-b + d)/(2.0*a);
		float tminus = (-b - d)/(2.0*a);

		// the two intersections are at the camera back
		if( (tplus < tmin) && (tminus < tmin) ){
			return false;
		}
		
		float t ;
		// the two intersections are at the camera front
		if( tminus > tmin) { 
			t = tminus;
		}
		
		// one intersection at the front. one at the back 
		if ((tplus > tmin) &&( tminus < tmin) ){
			t = tplus;
		}
		
		if ( t < h.getT() ){
			Vector3f normal = r.pointAtParameter(t)-center;
			normal = normal.normalized();
			h.set(t,material,normal);
			return true;
		}
		return false;
	}

protected:
	
	Vector3f center;
	float  radius;

};


#endif
