#include "SampleDebugger.h"

#include <cmath>

#include "Image.h"

#include <iostream>

using namespace std;

SampleDebugger::SampleDebugger( int width, int height, int nSamples ) :

    m_width( width ),
    m_height( height ),
    m_nSamples( nSamples )

{ 
    cout << "width = " << width << endl;
    cout << "height = " << height << endl;
    cout << "nSamples = " << nSamples << endl;
    assert( width > 0 && height > 0 && nSamples > 0 );
    samples = new Sample[ width * height * nSamples ];
}

SampleDebugger::~SampleDebugger()
{
    delete[] samples;
}

int SampleDebugger::getWidth()
{ 
    return m_width; 
}

int SampleDebugger::getHeight()
{ 
    return m_height; 
}

int SampleDebugger::getNumSamples()
{ 
    return m_nSamples;
}

Sample SampleDebugger::getSample( int i, int j, int n )
{
    return samples[ getIndex( i, j, n ) ]; 
}

void SampleDebugger::setSample( int x, int y, int i, const Vector2f& offset, const Vector3f& color )
{
    samples[ getIndex( x, y, i ) ].set( offset, color ); 
}

void SampleDebugger::renderSamples( char* samplesFile, int sampleZoom )
{
    // create an image
    Image image( m_width * sampleZoom, m_height * sampleZoom );

    for( int i = 0; i < m_width; ++i )
    {
        for( int j = 0; j < m_height; ++j )
        {
            // make a dark grey checkerboard pattern
            for( int zi = i * sampleZoom; zi < ( i + 1 ) * sampleZoom; ++zi)
            {
                for( int zj = j * sampleZoom; zj < ( j + 1 ) * sampleZoom; ++zj)
                {
                    if( ( ( i + j ) % 2 ) == 0 )
                    {
                        image.SetPixel( zi, zj, Vector3f( 0.2f, 0.2f, 0.2f) );
                    }
                    else 
                    {
                        image.SetPixel( zi, zj, Vector3f( 0, 0, 0 ) );
                    }
                }
            }

            // draw the sample points
            for( int n = 0; n < m_nSamples; ++n)
            {
                Sample s = getSample( i, j, n );
                Vector2f p = s.getPosition();
                assert( p[0] >= 0 && p[0] <= 1 );
                assert( p[1] >= 0 && p[1] <= 1 );

                int x = ( int )floor( ( i + p[0] ) * sampleZoom );
                int y = ( int )floor( ( j + p[1] ) * sampleZoom );
                assert( x >= 0 );
                assert (y >= 0);

                if( x >= m_width * sampleZoom )
                {
                    x = m_width * sampleZoom - 1;
                }

                if( y >= m_height * sampleZoom )
                {
                    y = m_height * sampleZoom - 1;
                }

                image.SetPixel( x, y, s.getColor() );
            }
        }
    }

    cout << " is this where ti's erroring? " << samplesFile << endl;
    // save the image
    image.SaveTGA( samplesFile );
}

int SampleDebugger::getIndex( int i, int j, int n )
{
    assert( i >= 0 && i < m_width );
    assert( j >= 0 && j < m_height );
    assert( n >= 0 && n < m_nSamples );
    return i * m_height * m_nSamples + j * m_nSamples + n; 
}

