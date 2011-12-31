#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight : public Light
{
public:

    DirectionalLight();
    
    DirectionalLight( const Vector3f& direction, const Vector3f& color );
    
    virtual ~DirectionalLight();

    virtual void getIllumination( const Vector3f& p,
            Vector3f& dir, Vector3f& col, float& distanceToLight ) const;

private:

    Vector3f m_direction;
    Vector3f m_color;

};

#endif // DIRECTIONAL_LIGHT_H
