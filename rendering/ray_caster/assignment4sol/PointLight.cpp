#include "PointLight.h"

PointLight::PointLight( const Vector3f& position, const Vector3f& color,
        float constantAttenuation,
        float linearAttenuation,
        float quadraticAttenuation ) :

    m_position( position ),
    m_color( color ),
    m_constantAttenuation( constantAttenuation ),
    m_linearAttenuation( linearAttenuation ),
    m_quadraticAttenuation( quadraticAttenuation )

{

}
    
PointLight::~PointLight()
{

}

// virtual
void PointLight::getIllumination( const Vector3f& p,
        Vector3f& dir, Vector3f& col, float& distanceToLight ) const
{
    dir = m_position - p;
    distanceToLight = dir.abs();
    dir.normalize();

    float attenuation = 1.0f /
        ( m_constantAttenuation +
          m_linearAttenuation * distanceToLight +
          m_quadraticAttenuation * distanceToLight * distanceToLight );

    if( attenuation < 0 )
    {
        attenuation = 0;
    }

    col = m_color * attenuation;
}

