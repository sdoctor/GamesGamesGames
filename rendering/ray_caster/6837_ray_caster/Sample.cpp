#include "Sample.h"

#include <cassert>

Sample::Sample() :

    m_position( 0.5f, 0.5f ),
    m_color( 0, 0, 0 )

{ 

}

Sample::~Sample()
{

}

Vector2f Sample::getPosition()
{
    return m_position;
}

Vector3f Sample::getColor()
{
    return m_color;
}

void Sample::set( const Vector2f& position, const Vector3f& color )
{ 
    assert( position[0] >= 0 && position[0] <= 1 );
    assert( position[1] >= 0 && position[1] <= 1 );

    m_position = position;
    m_color = color;
}
