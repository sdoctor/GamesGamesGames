#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;

class Triangle: public Object3D
{
public:
	Triangle();
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m):Object3D(m){
		this->a = a ;
		this->b = b ;
		this->c = c ;
		this->direction = Vector3f::cross( b-a , c-b).normalized();
	}
	virtual bool intersect( const Ray& ray,  Hit& hit , float tmin){
		
		Vector3f Rd = ray.getDirection();
		Vector3f Ro = ray.getOrigin();
		// no intersection
		if( Vector3f::dot( direction , Rd )==0 ){
			return false;}
		

		Matrix3f A( a[0]-b[0] , a[0]-c[0] ,Rd[0],
			    a[1]-b[1] , a[1]-c[1] ,Rd[1],
			    a[2]-b[2] , a[2]-c[2] ,Rd[2]);

		Vector3f b(a[0]-Ro[0] , a[1]-Ro[1],a[2]-Ro[2]);

		Vector3f x = A.inverse()*b;

		float beta = x[0], gamma = x[1], t = x[2];

		if( (beta>=0.0)&&(gamma>=0.0)&&(beta+gamma<=1)){
			if( (t < tmin) || (t>hit.getT())){ return false;}
			else{
				hit.set( t , this->material , direction);
				return true;
			}
		}
		return false;
	}	
	

protected:
	Vector3f a,b,c;
	Vector3f direction;
};

#endif //TRIANGLE_H
