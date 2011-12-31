#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "PerlinNoise.h"

class Material
{
public:
   
    Material( const Vector3f& diffuseColor )
    {
        diffuse_color = diffuseColor;
    }
    Material() { } // dummy constructor

    virtual ~Material(){}

    virtual Vector3f getDiffuseColor() const
    {return  diffuse_color;}
   
    virtual Vector3f getReflectiveColor() const;

    virtual Vector3f Shade( const Ray& ray, const Hit& hit,
            const Vector3f& dirToLight, const Vector3f& lightColor ) const = 0;

    static  Vector3f pointwiseDot( const Vector3f& v1 , const Vector3f& v2 ); 

    float clampedDot( const Vector3f& L , const Vector3f& N )const ;

protected:

    Vector3f diffuse_color;
    
    // TODO make sure your material classes store the reflective multipliers
    
};

class PhongMaterial: public Material
{
public:
    PhongMaterial( Vector3f diffuseColor , Vector3f specularColor , float exponent,
        Vector3f reflectiveColor, Vector3f transparentColor, float indexOfRefraction,
        char* texture);
        
    virtual Vector3f getDiffuseColor() const
    {return  diffuse_color;}
   
    virtual Vector3f getReflectiveColor() const
    {return reflective_color;}
    
       
    virtual Vector3f Shade( const Ray& ray, const Hit& hit,
            const Vector3f& dirToLight, const Vector3f& lightColor ) const;


private:
    
    Vector3f specular_color;
    float exponent;
    Vector3f reflective_color;
    Vector3f transparent_color;
    float index_of_refraction;
    char* texture;
};











#endif // MATERIAL_H
