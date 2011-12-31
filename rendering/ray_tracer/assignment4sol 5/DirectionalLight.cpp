#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() :

    m_direction( 0, 0, 0 ),
    m_color( 1, 1, 1 )

{

}

DirectionalLight::DirectionalLight( const Vector3f& direction, const Vector3f& color ) :

    m_direction( direction.normalized() ),
    m_color( color )
{

}

DirectionalLight::~DirectionalLight()
{

}

// virtual
void DirectionalLight::getIllumination( const Vector3f& p,
        Vector3f& dir, Vector3f& col, float& distanceToLight ) const
{
    // the direction to the light is the opposite of the
    // direction of the directional light source
    dir = -m_direction;
    col = m_color;

    distanceToLight = FLT_MAX; 
}
