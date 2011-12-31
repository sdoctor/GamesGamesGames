


#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

class PointLight : public Light
{
public:

    PointLight( const Vector3f& position, const Vector3f& color,
            float constantAttenuation,
            float linearAttenuation,
            float quadraticAttenuation );
    
    ~PointLight();

    virtual void getIllumination( const Vector3f& p,
            Vector3f& dir, Vector3f& col, float& distanceToLight ) const;

private:

    PointLight();

    Vector3f m_position;
    Vector3f m_color;

    float m_constantAttenuation;
    float m_linearAttenuation;
    float m_quadraticAttenuation;
};

#endif // POINT_LIGHT_H
