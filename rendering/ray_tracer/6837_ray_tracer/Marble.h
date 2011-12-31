/* 
 * File:   Marble.h
 * Author: drakeley
 *
 * Created on December 6, 2011, 5:13 PM
 */

#ifndef MARBLE_H
#define	MARBLE_H

#include "Material.h"



class Marble : public Material {
public:
    Marble(Matrix4f m, Material* m1, Material* m2, int octaves, float frequency, float amplitude ) : Material() {
        this->matrix = m;
        this->mat1 = m1;
        this->mat2 = m2;
        this->octaves = octaves;
        this->freq = frequency;
        this->amp = amplitude;
    }
   
    virtual ~Marble(){}
   
    virtual Vector3f getDiffuseColor() const{
        return diffuse_color;
    }
   
    Vector3f Shade(const Ray &ray, const Hit &hit, const Vector3f &dirToLight, const Vector3f &lightColor) const {
        Vector3f hp3 = ray.pointAtParameter(hit.getT());
        Vector4f hp4(hp3[0], hp3[1], hp3[2], 1);
        Matrix4f mat = matrix;
        Vector4f hitPoint = mat*hp4;
        
        float noisef = PerlinNoise::octaveNoise(Vector3f(hitPoint[0], hitPoint[1], hitPoint[2]), octaves);
        float newf = sin(freq*hitPoint[0] + amp*noisef);
      
        float r1 = -1;
        float r2 = 1;
        float t1 = 0;
        float t2 = 1;
        float scale = (r2-r1) / (t2 - t1);
        float res = (t1 + (newf - r1))/scale;
        
        newf = res;
        
        Vector3f mat1Col = mat1->Shade(ray, hit, dirToLight, lightColor);
        Vector3f mat2Col = mat2->Shade(ray, hit, dirToLight, lightColor);
        
        Vector3f lerp = Vector3f::lerp(mat2Col, mat1Col, newf);
        
        return lerp;
        
    }
   
protected:
    Matrix4f matrix;
    Material* mat1;
    Material* mat2;
    int octaves;
    float freq;
    float amp;
};


#endif	/* MARBLE_H */

