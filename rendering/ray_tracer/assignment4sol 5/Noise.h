/* 
 * File:   Noise.h
 * Author: drakeley
 *
 * Created on December 6, 2011, 5:10 PM
 */

#ifndef NOISE_H
#define	NOISE_H

#include "Material.h"



class Noise : public Material {
public:
    Noise(Matrix4f m, Material* m1, Material* m2, int oct) : Material() {
        this->matrix = m;
        this->mat1 = m1;
        this->mat2 = m2;
        this->octaves = oct;
    }
   
    virtual ~Noise(){}
   
    virtual Vector3f getDiffuseColor() const
    {
        return diffuse_color;
    }
   
    Vector3f Shade(const Ray &ray, const Hit &hit, const Vector3f&dirToLight, const Vector3f & lightColor) const {
        float noisef = 0;
        Vector3f hp3 = ray.pointAtParameter(hit.getT());
        Vector4f hp4(hp3[0], hp3[1], hp3[2], 1);
        Matrix4f mat = matrix;
        Vector4f hitPoint = mat*hp4;
        
        noisef = PerlinNoise::octaveNoise(Vector3f(hitPoint[0], hitPoint[1], hitPoint[2]), octaves);
        
        noisef += 0.5;
        if (noisef < 0) {
            noisef = 0;
        }
        if (noisef > 1) {
            noisef = 1;
        }
        
        Vector3f mat1Col = mat1->Shade(ray, hit, dirToLight, lightColor);
        Vector3f mat2Col = mat2->Shade(ray, hit, dirToLight, lightColor);
        
        Vector3f lerp = Vector3f::lerp(mat2Col, mat1Col, noisef);
        
        return lerp;
    }
   
protected:
    Material* mat1;
    Material* mat2;
    Matrix4f matrix;
    int octaves;
};


#endif	/* NOISE_H */

