/* 
 * File:   Wood.h
 * Author: drakeley
 *
 * Created on December 9, 2011, 11:48 AM
 */

#ifndef WOOD_H
#define	WOOD_H


#include "Material.h"



class Wood : public Material {
public:
    Wood(Matrix4f m, Material* m1, Material* m2, int octaves ) : Material() {
        
        this->matrix = m;
        this->mat1 = m1;
        this->mat2 = m2;
        this->octaves = octaves;

    }
   
    virtual ~Wood(){}
   
    virtual Vector3f getDiffuseColor() const{
        return diffuse_color;
    }
   
    Vector3f Shade(const Ray &ray, const Hit &hit, const Vector3f&dirToLight, const Vector3f & lightColor) const {
       float noise = 0;
   Vector3f hp3 = ray.pointAtParameter(hit.getT());
   Vector3f center = Vector3f(0,0,0);
   Vector3f dist = hp3 - center;
   Vector4f hp4(dist[0], dist[1], dist[2], 1);
   Vector4f hitPoint = matrix*hp4;
   
  
   
   float x = hitPoint[0]; // dist to center
   float y = hitPoint[1]; // dist to center

//    noise += 0.5;
//    if (noise < 0) {
//        noise = 0;
//    }
//    if (noise > 1) {
//        noise = 1;
//    }
//
   Vector3f mat1Col = mat1->Shade(ray, hit, dirToLight, lightColor);
   Vector3f mat2Col = mat2->Shade(ray, hit, dirToLight, lightColor);
   
   float rad = 0.1;
   float width = 0.005;
   float grain = sin(sqrt(x*x + y*y));
   float mod = abs((grain / rad) - int(grain / rad));
   if (mod < width) {
       return mat1Col;
   }
   else {
       return mat2Col;
   }
    }
   
protected:
    Vector3f center;
    Matrix4f matrix;
    Material* mat1;
    Material* mat2;
    int octaves;

};


#endif	/* WOOD_H */

