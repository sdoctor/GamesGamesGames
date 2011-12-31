#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

class Group:public Object3D
{
public:

	Group(){
		objectArray = NULL;
		this->num_objects = 0;
	}
	
	Group( int num_objects ){
		objectArray = new Object3D*[num_objects];
		this->num_objects = num_objects;
	}

	//~Group()

	virtual bool intersect( const Ray& r , Hit& h , float tmin ); 
	
	void addObject( int index , Object3D* obj );

	int getGroupSize(){ return num_objects ; }

private:
	int num_objects ; 

	Object3D** objectArray;
};

#endif
	
