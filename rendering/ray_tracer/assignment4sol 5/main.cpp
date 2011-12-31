#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include "RayTracer.h"
#include <string.h>
#include <vector>
#include "SampleDebugger.h"

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

int main( int argc, char* argv[] )
{
    // Fill in your implementation here.

    // This loop loops over each of the input arguments.
    // argNum is initialized to 1 because the first
    // "argument" provided to the program is actually the
    // name of the executable (in our case, "a4").
    for( int argNum = 1; argNum < argc; ++argNum )
    {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }
	
    int w, h ; //img size
    float depthMin, depthMax;
    char filename[80];
    char output[80];
    char depthOutput[80];
    char normalsOutput[80];
    bool depthMode = false, normalsMode = false, imageMode = false;

    int max_bounces = 0;
    float cutoff_weight;
    bool shadows = false;
    bool refraction = false;

    int uniform_samples = 0;
    int jitter_samples = 0;
    float box_filter_radius;

    bool render_samples = false;
    char* render_samples_outfile;
    int zoom_factor;

    for( int i = 0 ; i < argc ; i++)
    {
        if(!strcmp(argv[i], "-input")){
                strcpy(filename, argv[i+1]);
        }
        else if(!strcmp(argv[i], "-size")){
                w = atoi(argv[i+1]);
                h = atoi(argv[i+2]);
        }
        else if(!strcmp(argv[i], "-output")){
                strcpy(output , argv[i+1]);
                imageMode = true;
        }
        else if(!strcmp(argv[i], "-depth")){
                depthMode = true;
                depthMin = atof(argv[i+1]);
                depthMax = atof(argv[i+2]);
                strcpy(depthOutput , argv[i+3]);
        }
        else if(!strcmp(argv[i], "-normals")){
                normalsMode = true;
                strcpy(normalsOutput , argv[i+1]);
        }
        else if(!strcmp(argv[i], "-bounces")){
            max_bounces = atoi(argv[i+1]);
        }
        else if(!strcmp(argv[i], "-weight")){
            cutoff_weight = atoi(argv[i+1]);
        }
        else if(!strcmp(argv[i], "-shadows")){
            shadows = true;
        }
        else if(!strcmp(argv[i], "-uniform_samples")){
            uniform_samples = atoi(argv[i+1]);
        }
        else if(!strcmp(argv[i], "-jittered_samples")){
            jitter_samples = atoi(argv[i+1]);
        }
        else if(!strcmp(argv[i], "-box_filter")){
            box_filter_radius = atof(argv[i+1]);
        }
        else if(!strcmp(argv[i], "-render_samples")){
//            strcpy(render_samples_outfile, argv[i+1]);
            render_samples_outfile = argv[i+1];
            zoom_factor = atoi(argv[i+2]);
            render_samples = true;
        }
        else if (!strcmp(argv[i], "-refraction")){
            refraction = true;
        }
    }
	
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.

    SceneParser sp = SceneParser(filename);

    RayTracer rt = RayTracer(&sp, max_bounces, cutoff_weight, shadows, refraction); 

    Image image( w , h );
    Image depth( w , h );
    Image normals( w,h ); 

    Camera* camera = sp.getCamera();

    // Variables for anti-aliasing
    SampleDebugger *sd;
    
    Hit hit;
    int num_samples = max(uniform_samples, jitter_samples);

    if (render_samples)
    {    
//        cout << "render samples - now making the sample_debugger" << endl;
        sd = new SampleDebugger(w, h, num_samples);
    }

//    cout << "now starting iteration through pixels" << endl;
    for( int  j = 0 ; j < h ; j++ )
    {
        for ( int i = 0 ; i < w ; i++ )
        {
//            if (i > 144 && j > 43) {cout << "at beginning of loop i = " << i<< "j = " << j << endl;}
            
            Vector3f pixelColor;
            Vector3f normalVal;
            
            
//            if (i > 144 && j > 43) {cout << " checking num_samples" << endl;}
            if (num_samples > 0)
            {
                float grid_width = sqrt(num_samples); 
                float max_offset = 1.0/grid_width;
                float offset = 0;
                
//                if (i > 144 && j > 43) {cout << " where is this getting stuck - jitter samples?" << endl;}
                if (jitter_samples > 0)
                {
                    offset += (float)rand()/RAND_MAX * max_offset;
                }
                int count = 0;
                Vector3f color_sum = Vector3f(0.0, 0.0, 0.0);


//                if (i > 144 && j > 43) {cout << " where is this getting stuck - for loop?" << endl;}
                for (int grid_x = 0; grid_x < grid_width; grid_x++)
                {
                    for (int grid_y = 0; grid_y < grid_width; grid_y++)
                    {
//                        if (i > 144 && j > 43) {cout << " in second for loop: grid_x = " << grid_x << "grid y =" << grid_y << endl;}
                        float xin = grid_x*max_offset + i + offset;
                        float yin = grid_y*max_offset + j + offset;

                        float normX = (float)((float)(xin-((float)w)/2))/((float)w/2);
                        float normY = (float)((float)(yin-((float)h)/2))/((float)h/2);

                        Ray ray = camera->generateRay( Vector2f( normX , normY ) );
                        
                        hit = Hit(INFINITY, NULL, Vector3f(1,0,0));
                        Vector3f local_color = rt.traceRay(ray, camera->getTMin(), max_bounces, cutoff_weight, hit);
                        color_sum += local_color;
                        
//                        if (i > 144 && j > 43) {cout << " where is this getting stuck first render sampels?" << endl;}
                        if (render_samples)
                        {
                            cout << "1) count = " << count << endl;
                            Vector2f offset_vec = Vector2f(max_offset*grid_x+offset, max_offset*grid_y+offset);
                            sd->setSample(i, j, count, offset_vec, local_color);
                            count++;
                        }

                    }

                }
//                if (i > 144 && j > 43) {cout << " where is this getting stuck - setting pixel color?" << endl;}
                pixelColor = color_sum/num_samples;
                
            }

            else
            {
//                float x = 2*((float)j/((float)w - 1.0f)) - 1.0f;
//                float y = 2*((float)i/((float)h - 1.0f)) - 1.0f;

                float x = (float)((float)(i+0.25-((float)w)/2))/((float)w/2);
                float y = (float)((float)(j+0.25-((float)h)/2))/((float)h/2);
                
                
                Ray ray = camera->generateRay( Vector2f( x , y ) );

//                        if (i > 144 && j > 43) {cout << " where is this getting stuck - tracing the ray?" << endl;}

                
                //			group->intersect( ray , hit , camera->getTMin() ) ; 			

                hit = Hit(INFINITY, NULL, Vector3f(1,0,0));
                Vector3f color_normal = rt.traceRay(ray, camera->getTMin(), max_bounces, cutoff_weight, hit);

//                if (i > 144 && j > 43) {cout << " made it through traceRay?" << endl;}
                pixelColor = color_normal;
                
//			if( hit.getMaterial()==NULL){ //background
//				
//				pixelColor = Scene.getBackgroundColor();
//				normalVal = Vector3f(0.0,0.0,0.0);
//			}
//			else{
//				//ambient light
//				pixelColor = PhongMaterial::pointwiseDot( Scene.getAmbientLight(), hit.getMaterial()->getDiffuseColor());
//				//defussion light
//				for( int i = 0 ; i < Scene.getNumLights(); i++){
//					Light* light = Scene.getLight(i);
//					Vector3f dirToLight, lightColor ;
//					Vector3f position = ray.pointAtParameter(hit.getT());
//					float dist = hit.getT(); 
//					light->getIllumination( position , dirToLight , lightColor , dist);
//
//					pixelColor += hit.getMaterial()->Shade( ray , hit , dirToLight , lightColor ) ; 
//				}
//				
//				//normal map			
//				Vector3f n = hit.getNormal();
//				normalVal = Vector3f( abs(n[0]),abs(n[1]),abs(n[2]));
//			}
            }
            float d = clampedDepth( hit.getT(), depthMin , depthMax);

//            cout << "setting pixel for i,j = " << i << ", " << j << endl;
            depth.SetPixel( i , j , Vector3f(d,d,d));
            image.SetPixel( i ,  j , pixelColor );
            
//            if (i > 144) {cout << "where is this getting stuck?" << endl;}
            
            normalVal = hit.getNormal();
            for (int k = 0; k < 3; k++)
            {
//                if (i > 144) {cout << "where is this getting stuck? in normals?" << endl;}
                normalVal[k] = fabs(normalVal[k]);
            }
//            if (i > 144) {cout << "where is this getting stuck? setting normals?" << endl;}
            normals.SetPixel( i , j , normalVal) ; 
//            if (i > 144) {cout << "where is this getting stuck? redner samples??" << endl;}

//            if (i > 144) {cout << "where is this getting stuck? before starting the next loop?" << endl;}

        }
    }

    cout << "output = " << output << "should not be null!" << endl;
    if(imageMode){image.SaveTGA(output);}
    if( depthMode){ depth.SaveTGA(depthOutput);}
    if( normalsMode){ normals.SaveTGA(normalsOutput);}

    if (render_samples)
    {
        sd->renderSamples(render_samples_outfile, zoom_factor);
    }
    return 0;
}

float clampedDepth( float depthInput, float depthMin , float depthMax ){

	float output; 
	if( depthInput < depthMin)
		output = 1.0f;
	else if (depthInput > depthMax )
		output = 0.0f;
	else 
		output = (depthMax - depthInput)/(depthMax - depthMin);
	
	return output;
}


