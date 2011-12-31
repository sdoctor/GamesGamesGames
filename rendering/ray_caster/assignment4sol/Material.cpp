#include "Material.h"


Vector3f Material::Shade(const Ray& ray, const Hit& hit, const Vector3f& dirToLight, const Vector3f& lightColor) const
{
    //Diffuse Shading
    Vector3f color = clampedDot( dirToLight , hit.getNormal() )*pointwiseDot( lightColor , this->diffuse_color);
    return color ;
}

Vector3f Material::getReflectiveColor() const
{
    return Vector3f(0,0,0);
}

Vector3f Material::pointwiseDot(const Vector3f& v1, const Vector3f& v2)
{
    Vector3f out = Vector3f( v1[0]*v2[0] , v1[1]*v2[1] , v1[2]*v2[2]);
    return out;
}

float Material::clampedDot(const Vector3f& L, const Vector3f& N) const
{
    float d = Vector3f::dot(L,N);
    return (d>0)? d : 0 ;
}

// TODO get reflective material method

//**************************************************************************

PhongMaterial::PhongMaterial(Vector3f diffuseColor, Vector3f specularColor, 
        float exponent, Vector3f reflectiveColor, Vector3f transparentColor, 
        float indexOfRefraction, char* texture):Material(diffuse_color)
{
    this->diffuse_color = diffuseColor;
    this->specular_color = specularColor ;
    this->exponent = exponent ;
    this->reflective_color = reflectiveColor;
    this->transparent_color = transparentColor;
    this->index_of_refraction = indexOfRefraction;
    this->texture = texture;
}

Vector3f PhongMaterial::Shade(const Ray& ray, const Hit& hit, 
        const Vector3f& dirToLight, const Vector3f& lightColor) const
{
    // we just want to return D_i + S_i
    Vector3f Di = Material::Shade(ray, hit, dirToLight, lightColor);
    
    
    if (clampedDot(dirToLight, hit.getNormal()) == 0)
    {
        return Di;
    }
    
    // half angle vector is the vector that bisects the vector pointing towards 
    // the light and the vector pointing towards the camera
    Vector3f half_angle_vector = Vector3f(dirToLight - ray.getDirection()).normalized();
    
//    cout << "exponent = " << exponent << endl;
    
    Vector3f specular_term = pointwiseDot(lightColor,this->specular_color)*pow(clampedDot(half_angle_vector, hit.getNormal()), this->exponent);
    
//    cout << "specular term =" << specular_term[0] << specular_term[1] << specular_term[2] << endl;
    return Di + specular_term;
    
}

//**************************************************************************

