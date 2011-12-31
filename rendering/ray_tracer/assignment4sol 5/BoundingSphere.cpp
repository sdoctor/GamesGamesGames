

#include "BoundingSphere.h"
bool BoundingSphere::intersect(const Ray& r, Hit& h, float tMin)
{
    Hit newHit = Hit(INFINITY, NULL, Vector3f(1,0,0) );
     if (Sphere::intersect(r, newHit, tMin)) {
         return object->intersect(r, h, tMin);
     }
     return false;
    
}

