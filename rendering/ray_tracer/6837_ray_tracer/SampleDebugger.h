#ifndef SAMPLE_DEBUGGER_H
#define SAMPLE_DEBUGGER_H

#include <cassert>

#include "Sample.h"

class Filter;

// A helper class for supersampling.  Stores a set of samples for
// each pixel of the final image.

class SampleDebugger
{
public:

    SampleDebugger( int width, int height, int nSamples );
    ~SampleDebugger();

    // ACCESSORS
    int getWidth();
    int getHeight();
    int getNumSamples();

    Sample getSample( int i, int j, int n );
    void setSample( int x, int y, int i,  const Vector2f& offset, const Vector3f& color );

    void renderSamples( char* samplesFile, int sampleZoom );
  
private:

    int getIndex( int i, int j, int n );

    int m_width;
    int m_height;
    int m_nSamples;
    Sample* samples;

};

#endif // SAMPLE_DEBUGGER_H
