#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"

class Transform: public Object3D
{
public: 
	Transform(){}
	Transform( const Matrix4f& m, Object3D* o ){
		this->m = m;
		this->o = o;
	}
	~Transform(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin){

		Matrix4f mInv = m.inverse();

		// get the new ray
		Vector4f localOrig = mInv*Vector4f( r.getOrigin(),1.0f);
		Vector4f localDir = mInv*Vector4f( r.getDirection(),0.0f);
		Ray localRay(  localOrig.xyz() , localDir.xyz());

		if ( o->intersect( localRay , h , tmin)){

			// multiply the normal by inverse transpose
			Matrix3f m2 = m.getSubmatrix3x3(0,0);
			Vector3f newNorm = m2.inverse().transposed()*h.getNormal();
			newNorm = newNorm.normalized();

			h.set( h.getT() , h.getMaterial() , newNorm );

			return true;
		}
		else{ return false;}
	}

protected:

	Matrix4f m; // transform matrix
	Object3D* o; //un-transformed object

	
};

#endif //TRANSFORM_H
