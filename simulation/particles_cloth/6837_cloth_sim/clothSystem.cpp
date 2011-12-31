#include "clothSystem.h"
#include <iostream>
#include <assert.h>

/*
 * Cloth system for arbitrary width and height, to make width*height num_particles
 * Begins flat and falls according to gravity, viscous drag, and spring forces
 * 
 */
Cloth::Cloth(INTEGRATOR_TYPE integrator_type, int width, int height):PendulumSystem()
{
    this->integrator_type = integrator_type;
    this->width = width;
    this->height = height;
    num_particles = width * height;

    stationary_particles.push_back(0); // index of particles to remain fixed
    stationary_particles.push_back(width-1);

    vector<Vector3f> init_state;

    // fill in code for initializing the state based on the number of particles
    for (int i = 0; i < width; i++) 
    {
        for (int j = 0; j < height; j++)
        {
            // sets position of particles in grid along x and z
            Vector3f pos = Vector3f(0.5*j, 0, 0.5*i); 
            Vector3f velocity = Vector3f(0, 0, 0);

            init_state.push_back(pos);
            init_state.push_back(velocity);

            // Hook up springs
            attachSprings(i, j);
        }
    }    
    initializeParticlesToSprings(); 
    assert (springs_of_particles.size() == (unsigned) num_particles); //invariant

    setState(init_state); 
}

/* Returns the particle index of a point at position (i,j) in grid */
int Cloth::indexOf(int i, int j)
{
    return i * width + j;
}

int Cloth::jOf(int particle_index)
{
    return particle_index % height;
}
int Cloth::iOf(int particle_index, int j)
{
    if (particle_index > j*width ) // last element
    {
        return width-1;
    }
    else
    {
        return (particle_index - j) % (width - 1);
    }
}

/* Attaches springs for particle at (i,j) in cloth
 * 
 * Including: 
 * structural springs (to adjacent neighbors)
 * shear springs (to diagonal neighbors)
 * flex springs (to adjacent neighbors 2 over)
 */
void Cloth::attachSprings(int i, int j)
{
    // Name the indices to make it readable and debuggable
    int particle_index = indexOf(i, j);
    int below_neighb = indexOf(i+1, j);
    int right_neighb = indexOf(i, j+1);
    int diag_left_neighb = indexOf(i+1, j-1);
    int diag_right_neighb = indexOf(i+1, j+1);
    int right_neighb_two_over = indexOf(i, j+2);
    int bottom_neighb_two_down = indexOf(i+2, j);
    
    // Structural Springs
    if (!Spring::bottomEdge(height, i)) // not on bottom edge 
    {
        // add vertical structural spring
        if (!Spring::springInSprings(springs, particle_index, below_neighb))
        {
            springs.push_back(Spring(SPRING_CONSTANT, SPRING_LENGTH, particle_index, below_neighb));
        }        
    }
    if (!Spring::rightEdge(width, j)) // not on right edge 
    {
        // add horizontal structural spring
        if (!Spring::springInSprings(springs, particle_index, right_neighb))
        {
            springs.push_back(Spring(SPRING_CONSTANT, SPRING_LENGTH, particle_index, right_neighb));
        }        
    }
    
    // Shear springs
    if (!Spring::rightEdge(width, j) and !Spring::bottomEdge(height, i)) // not on right or bottom edge 
    {
        // add right diagonal shear spring
        if (!Spring::springInSprings(springs, particle_index, diag_right_neighb))
        {
            springs.push_back(Spring(SPRING_CONSTANT, sqrt(2)*SPRING_LENGTH, particle_index, diag_right_neighb));
        }        
    }   
    if (!Spring::leftEdge(width, j) and !Spring::bottomEdge(height, i)) // not left or bottom edge
    {        
        // add left diagonal shear spring
        if (!Spring::springInSprings(springs, particle_index, diag_left_neighb))
        {
            springs.push_back(Spring(SPRING_CONSTANT, sqrt(2)*SPRING_LENGTH, particle_index, diag_left_neighb));
        }
    }

    // Flex springs        
    if (!Spring::bottomEdge(height, i+1) and !Spring::bottomEdge(height, i)) // not two from bottom edge 
    {
        // add vertical flex spring
        if (!Spring::springInSprings(springs, particle_index, bottom_neighb_two_down))
        {
            springs.push_back(Spring(SPRING_CONSTANT, 2*SPRING_LENGTH, particle_index, bottom_neighb_two_down));
        }        
    }   
    if (!Spring::rightEdge(width, j+1) and !Spring::rightEdge(width, j)) // not two from right edge 
    {
        // add horizontal flex spring
        if (!Spring::springInSprings(springs, particle_index, right_neighb_two_over))
        {
            springs.push_back(Spring(SPRING_CONSTANT, 2*SPRING_LENGTH, particle_index, right_neighb_two_over));
        }
    }
}



//
//// render the system (ie draw the particles)
//void Cloth::draw()
//{
//      for (int i = 0; i < num_particles; i++) {      
//	  Vector3f pos = PositionFinder::getPosition(getState(), i);
//          if (!wireframe)
//          {
//              glPushMatrix();
//		glTranslatef(pos[0], pos[1], pos[2] );
//		glutSolidSphere(0.075f,10.0f,10.0f); 
//                
//                      
//                
//                if (i < num_particles-1)
//                {
//                    cout << "current particle = " << i << endl;
//                    cout << "particle " << i <<"'s j = " << jOf(i) << ", i = " << iOf(i, jOf(i)) << endl;
//                    
//                    int h = jOf(i);
//                    int w = iOf(i, h);
//                    
//                    if (h<2 and w<2)
//                    {
//                        Vector3f right_neighb = PositionFinder::getPosition(getState(), indexOf(h+1, w));
//                        Vector3f below_neighb = PositionFinder::getPosition(getState(), indexOf(h, w+1));
//
//                        glBegin(GL_TRIANGLES);
//                        Vector3f norm = right_neighb * pos;
//                        glNormal3f(norm[0], norm[1], norm[2]);
//                        glVertex3f(pos[0], pos[1], pos[2]);
//                        glNormal3f(norm[0], norm[1], norm[2]);
//                        glVertex3f(right_neighb[0], right_neighb[1], right_neighb[2]);
//                        glNormal3f(norm[0], norm[1], norm[2]);
//                        glVertex3f(below_neighb[0], below_neighb[1], below_neighb[2]);
//                    }
//
//                    glEnd();
//                }
//                
//		glPopMatrix();
//          }              
//	}
//        Spring::drawSprings(getState(), springs);
//}
//
