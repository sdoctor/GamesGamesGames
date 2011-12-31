#ifndef SAMPLE_H
#define SAMPLE_H

#include <vecmath.h>

// A helper class for supersampling.  The position specifies the 2D
// offset within the pixel from (0,0) -> (1,1).

class Sample
{
public:

    Sample();
    ~Sample();

    Vector2f getPosition();
    Vector3f getColor();

    void set( const Vector2f& position, const Vector3f& color );

private:

    Vector2f m_position;
    Vector3f m_color;

};

#endif
