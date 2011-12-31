#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include <GL/glut.h>
#include <vecmath.h>
#include "camera.h"
#include "integrator.h"
#include "simpleSystem.h"
#include "pendulumSystem.h"
#include "chainSystem.h"
#include "clothSystem.h"

using namespace std;

enum SYSTEM_TYPE {SIMPLE, PENDULUM, CHAIN, CLOTH};

// returns a random floating point number in [0,1]
float nextFloat()
{
    return float( rand() ) / RAND_MAX;
}

// Globals here.
namespace
{
    ParticleSystem *system;
    
    float stepsize = 0.02f;
    SYSTEM_TYPE system_type = CLOTH; // default
    INTEGRATOR_TYPE integrator_type = TRAPEZOID; //default

    // initialize your particle systems
    void initSystem(SYSTEM_TYPE sys_type, INTEGRATOR_TYPE integration_type)
    {
        // seed the random number generator with the current time
        srand( time( NULL ) );
        
        
        switch(sys_type)
        {
            case SIMPLE:
                system = new SimpleSystem(integration_type, 1);
                break;
        
            case PENDULUM:
                system = new PendulumSystem(integration_type, 2);
                break;
                
            case CHAIN:
                system = new Chain(integration_type, 5);         
                break;
                
            case CLOTH:
                system = new Cloth(integration_type, 8,8);
                break;
                
            default:
                cout << "Some error in initializing system" << endl;
        }
		
    }

    // Take a step forward for the particle shower
    void stepSystem()
    {
        system->stepSystem(stepsize);
    }

    // Draw the current particle positions
    void drawSystem()
    {
        // Base material colors (they don't change)
        GLfloat particleColor[] = {0.4f, 0.7f, 1.0f, 1.0f};
        GLfloat floorColor[] = {1.0f, 0.0f, 0.0f, 1.0f};

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, particleColor);

        glutSolidSphere(0.1f,10.0f,10.0f);

        system->draw();

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floorColor);
        glPushMatrix();
        glTranslatef(0.0f,-5.0f,0.0f);
        glScaled(50.0f,0.01f,50.0f);
        glutSolidCube(1);
        glPopMatrix();

    }
        

    //-------------------------------------------------------------------
    
        
    // This is the camera
    Camera camera;

    // These are state variables for the UI
    bool g_mousePressed = false;

    // Declarations of functions whose implementations occur later.
    void arcballRotation(int endX, int endY);
    void keyboardFunc( unsigned char key, int x, int y);
    void specialFunc( int key, int x, int y );
    void mouseFunc(int button, int state, int x, int y);
    void motionFunc(int x, int y);
    void reshapeFunc(int w, int h);
    void drawScene(void);
    void initRendering();

    // This function is called whenever a "Normal" key press is
    // received.
    void keyboardFunc( unsigned char key, int x, int y )
    {
        switch ( key )
        {
        case 27: // Escape key
            exit(0);
            break;
        case ' ':
        {
            Matrix4f eye = Matrix4f::identity();
            camera.SetRotation( eye );
            camera.SetCenter( Vector3f::ZERO );
            break;
        }
        
        case 's': // shake!
        {
            system->shake();
            break;
        }
        
        case 'w': //wireframe
        {
            system->toggle_wireframe();
            break;
        }
        
        case 'b': // make wind blow!
        {
            system->toggle_wind();
            break;
        }
        
        case 'r': // refresh
        {
            initSystem(system_type, integrator_type);
            break;

        }
        
        default:
            cout << "Unhandled key press " << key << "." << endl;        
        }

        glutPostRedisplay();
    }

    // This function is called whenever a "Special" key press is
    // received.  Right now, it's handling the arrow keys.
    void specialFunc( int key, int x, int y )
    {
        switch ( key )
        {
        
        }

        //glutPostRedisplay();
    }

    //  Called when mouse button is pressed.
    void mouseFunc(int button, int state, int x, int y)
    {
        if (state == GLUT_DOWN)
        {
            g_mousePressed = true;
            
            switch (button)
            {
            case GLUT_LEFT_BUTTON:
                camera.MouseClick(Camera::LEFT, x, y);
                break;
            case GLUT_MIDDLE_BUTTON:
                camera.MouseClick(Camera::MIDDLE, x, y);
                break;
            case GLUT_RIGHT_BUTTON:
                camera.MouseClick(Camera::RIGHT, x,y);
            default:
                break;
            }                       
        }
        else
        {
            camera.MouseRelease(x,y);
            g_mousePressed = false;
        }
        glutPostRedisplay();
    }

    // Called when mouse is moved while button pressed.
    void motionFunc(int x, int y)
    {
        camera.MouseDrag(x,y);        
    
        glutPostRedisplay();
    }

    // Called when the window is resized
    // w, h - width and height of the window in pixels.
    void reshapeFunc(int w, int h)
    {
        camera.SetDimensions(w,h);

        camera.SetViewport(0,0,w,h);
        camera.ApplyViewport();

        // Set up a perspective view, with square aspect ratio
        glMatrixMode(GL_PROJECTION);

        camera.SetPerspective(50);
        glLoadMatrixf( camera.projectionMatrix() );
    }

    // Initialize OpenGL's rendering modes
    void initRendering()
    {
        glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
        glEnable(GL_LIGHTING);     // Enable lighting calculations
        glEnable(GL_LIGHT0);       // Turn on light #0.

        glEnable(GL_NORMALIZE);

        // Setup polygon drawing
        glShadeModel(GL_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

        // Clear to black
        glClearColor(0,0,0,1);
    }

    // This function is responsible for displaying the object.
    void drawScene(void)
    {
        // Clear the rendering window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode( GL_MODELVIEW );  
        glLoadIdentity();              

        // Light color (RGBA)
        GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
        GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
        glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

        glLoadMatrixf( camera.viewMatrix() );

        // THIS IS WHERE THE DRAW CODE GOES.

        drawSystem();

        // This draws the coordinate axes when you're rotating, to
        // keep yourself oriented.
        if( g_mousePressed )
        {
            glPushMatrix();
            Vector3f eye = camera.GetCenter();
            glTranslatef( eye[0], eye[1], eye[2] );

            // Save current state of OpenGL
            glPushAttrib(GL_ALL_ATTRIB_BITS);

            // This is to draw the axes when the mouse button is down
            glDisable(GL_LIGHTING);
            glLineWidth(3);
            glPushMatrix();
            glScaled(5.0,5.0,5.0);
            glBegin(GL_LINES);
            glColor4f(1,0.5,0.5,1); glVertex3f(0,0,0); glVertex3f(1,0,0);
            glColor4f(0.5,1,0.5,1); glVertex3f(0,0,0); glVertex3f(0,1,0);
            glColor4f(0.5,0.5,1,1); glVertex3f(0,0,0); glVertex3f(0,0,1);

            glColor4f(0.5,0.5,0.5,1);
            glVertex3f(0,0,0); glVertex3f(-1,0,0);
            glVertex3f(0,0,0); glVertex3f(0,-1,0);
            glVertex3f(0,0,0); glVertex3f(0,0,-1);

            glEnd();
            glPopMatrix();

            glPopAttrib();
            glPopMatrix();
        }
                 
        // Dump the image to the screen.
        glutSwapBuffers();
    }

    void timerFunc(int t)
    {
        stepSystem();

        glutPostRedisplay();

        glutTimerFunc(t, &timerFunc, t);
    }
    
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char* argv[] )
{
    glutInit( &argc, argv );
    
    // We're going to animate it, so double buffer 
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 600, 600 );
    
    camera.SetDimensions( 600, 600 );

    camera.SetDistance( 10 );
    camera.SetCenter( Vector3f::ZERO );
    
    glutCreateWindow("Assignment 4");

    // Initialize OpenGL parameters.
    initRendering();

    // TODO make these command line arguments
    // First command line argument is the system type
    
    
    if (argc > 1)
    {
        if (string(argv[1]) == "e") {integrator_type = EULER;}
        else if (string(argv[1]) == "t"){integrator_type = TRAPEZOID;}  
        else {cout << "WHOOPs! Did not specify a correct integrator type! You said: " << argv[1] << endl;}
        
        stepsize = atof(argv[2]);
        
        if (argc > 2)// optional parameters
        {
            // opted not to do the highlight particular particle spring
            if (string(argv[3]) == "simple") {system_type = SIMPLE;}
            else if (string(argv[3]) == "pendulum"){system_type = PENDULUM;}  
            else if (string(argv[3]) == "chain"){system_type = CHAIN;}
            else if (string(argv[3]) == "cloth"){system_type = CLOTH;}
            else {cout << "WHOOPS! Did not specify a correct system type! you said: " << argv[3] << endl;}
        }
    }
    // Setup particle system
    initSystem(system_type, integrator_type);

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

    // Set up callback functions for mouse
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);

    // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Trigger timerFunc every 20 msec
    glutTimerFunc(20, timerFunc, 20);

        
    // Start the main loop.  glutMainLoop never returns.
//    cout << "Starting the glut main loop" << endl;
    glutMainLoop();

    return 0;	// This line is never reached.
}
