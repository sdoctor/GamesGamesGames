#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
using namespace std;

class Plane: public Object3D
{
public:
	Plane(){}
	Plane( const Vector3f& normal , float d , Material* m):Object3D(m){
		this->normal = normal;
		this->d = d ; 
	}
	~Plane(){}
	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		
		// ray is parallel to the plane = no intersection
		if( Vector3f::dot(normal,r.getDirection())==0){ 
			return false;
		}

		// find if it intersects with the plane
		float t = (d - Vector3f::dot(normal,r.getOrigin()))/Vector3f::dot(normal,r.getDirection());
		
		if( ( t > tmin)&&( t < h.getT() )){ 
			h.set( t , this->material , normal);
			return true;
		}
		return false;
	}

		


protected:
	Vector3f normal;
	float d;
};
#endif //PLANE_H
		

