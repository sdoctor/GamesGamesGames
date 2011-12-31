#include "RayTracer.h"


float EPSILON = .0001;

RayTracer::RayTracer(SceneParser *scene_parser, int max_bounces, 
        float cutoff_weight, bool shadows, bool refraction) 
{
    this->scene_parser = scene_parser;
    this->max_bounces = max_bounces;
    this->cutoff_weight = cutoff_weight;
    this->shadows = shadows;
    this->refraction = refraction;
}


Vector3f RayTracer::traceRay(Ray &ray, float tMin, int bounces, 
        float weight, Hit &hit) const 
{ 
    
    Group *objects = scene_parser->getGroup();
    objects->intersect(ray, hit, EPSILON);
 
    if(hit.getMaterial() == NULL) 
    {
        return scene_parser->getBackgroundColor();
    }
    
    Vector3f hit_point = ray.pointAtParameter(hit.getT());

    Vector3f ambient_light = scene_parser->getAmbientLight();
    Vector3f intial_color = hit.getMaterial()->getDiffuseColor();
	      
    Vector3f final_color = intial_color*ambient_light;    

    // Add up the color contribution from each light
    Vector3f dir_to_light, light_color;
    int num_lights = scene_parser->getNumLights();
    for(int i = 0; i < num_lights; i++)
    {

	float my_float = 0.0f;
	scene_parser->getLight(i)->getIllumination(hit_point, dir_to_light, light_color, my_float);

        // Find out if the hit point is in shadow and set bool
        bool in_shadow = false;
        if(shadows)
        {
	    Hit shadow_hit = Hit(INFINITY, NULL, Vector3f(1, 0, 0));
	    Ray shadow_ray = Ray(hit_point, dir_to_light );
	    objects->intersect(shadow_ray, shadow_hit, EPSILON);
	    if(shadow_hit.getMaterial() != NULL){
                if (shadow_hit.getT() < my_float) {
                        in_shadow = true;
                }
	    }
            
	}
    
        // Figure out the shading for the point
        if (!in_shadow)
        {
            Vector3f hit_color = hit.getMaterial()->Shade(ray, hit, dir_to_light, light_color);
            for(int k = 0; k < 3; k++){
                if(hit_color[k] < 0) hit_color[k] = 0;
                if(hit_color[k] > 1) hit_color[k] = 1;
            }
            final_color += hit_color; 
        }
    }
    
    // Figure out reflection for the point
    bool reflective = (hit.getMaterial()->getReflectiveColor().absSquared() > 0);
    if (bounces > 0 && reflective) 
    { 
        Vector3f bounce_direction = RayTracer::mirrorDirection(hit.getNormal(), ray.getDirection());
        Ray bounce_ray = Ray(hit_point, bounce_direction); 
        Hit bounce_hit = Hit(INFINITY, NULL, Vector3f(1, 0, 0));
        if (weight > cutoff_weight) 
        {
          weight = cutoff_weight;
        }
        scene_parser->getGroup()->intersect(bounce_ray, bounce_hit, EPSILON);
        Vector3f bounce_color = traceRay(bounce_ray, EPSILON, bounces-1, weight*hit.getMaterial()->getReflectiveColor().abs(), bounce_hit);
        final_color += bounce_color*hit.getMaterial()->getReflectiveColor();
    }
    
    if (refraction)
    {
        
    }
    
    
    return final_color;
}


Vector3f RayTracer::mirrorDirection(const Vector3f& normal, const Vector3f& incoming) 
{
	return incoming - 2*Vector3f::dot(incoming, normal)*normal;
}