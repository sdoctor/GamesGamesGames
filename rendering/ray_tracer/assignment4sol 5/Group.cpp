# include "Group.h"

bool Group::intersect(const Ray& r, Hit& h, float tmin)
{	
    bool result = false;
    for( int i = 0 ; i < num_objects ; i++){
            result += objectArray[i]->intersect( r , h , tmin);
    }
    return result;
}

void Group::addObject(int index, Object3D* obj)
{
    if ( index >= num_objects ){ 
            cerr<<" Group.h: index exceeds the scope of num_objects"<<endl;
            return ;
    }
    objectArray[index] = obj ; 
}