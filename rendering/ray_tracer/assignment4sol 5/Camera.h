#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 
	
	virtual float getTMin() const = 0 ; 

protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;

};


class OrthographicCamera: public Camera
{
public:
	OrthographicCamera( Vector3f center, Vector3f direction , Vector3f up , float size ){
		this->center = center;
		this->direction = direction.normalized();	
		this->horizontal = Vector3f::cross( direction , up ).normalized();
		// need to make an orthonormal vector to the projection
		this->up = Vector3f::cross(horizontal, direction).normalized(); 
		this->size = size;
	}

	virtual Ray generateRay( const Vector2f& point ){

		Vector3f rayOrigin = this->center + point[0]*size*horizontal/2 + point[1]*size*up/2;
		return Ray( rayOrigin , this->direction );
	}

	virtual float getTMin() const {
		return -1.0*FLT_MAX;
	}

private:
	float size ; 
};


class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle){
		this->center = center;
		this->direction = direction.normalized();
		this->horizontal = Vector3f::cross( direction , up ).normalized();
		this->up = Vector3f::cross(horizontal, direction).normalized(); 
		this->angle = angle;
	}

	virtual Ray generateRay( const Vector2f& point){
		
		float d = 1/tan(angle/2.0);
		Vector3f newDir = d*direction + point[0]*horizontal + point[1]*up;
		newDir = newDir.normalized();

		return Ray(center, newDir);
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
	float angle;
	


};

#endif //CAMERA_H
