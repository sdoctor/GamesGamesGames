#ifndef LIGHT_H
#define LIGHT_H

#include <Vector3f.h>

#include "Object3D.h"

class Light
{
public:

    Light()
    {

    }

    virtual ~Light()
    {

    }

    virtual void getIllumination( const Vector3f& p, Vector3f& dir, Vector3f& col, float& distanceToLight ) const = 0;

};

//class DirectionalLight : public Light
//{
//public:
//
//    DirectionalLight( const Vector3f& d, const Vector3f& c )
//    {
//        direction = d.normalized();
//        color = c;
//    }
//
//    ~DirectionalLight()
//    {
//
//    }
//
//    virtual void getIllumination( const Vector3f& p, Vector3f& dir, Vector3f& col, float& distanceToLight ) const
//    {
//        // the direction to the light is the opposite of the
//        // direction of the directional light source
//        dir = -direction;
//        col = color;
//    }
//
//private:
//
//    DirectionalLight(); // don't use
//
//    Vector3f direction;
//    Vector3f color;
//
//};

#endif // LIGHT_H
