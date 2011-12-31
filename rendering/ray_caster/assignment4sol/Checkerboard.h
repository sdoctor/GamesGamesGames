/* 
 * File:   Checkerboard.h
 * Author: drakeley
 *
 * Created on December 6, 2011, 5:10 PM
 */

#ifndef CHECKERBOARD_H
#define	CHECKERBOARD_H

#include "Material.h"

class Checkerboard : public Material {
public:
    Checkerboard(Matrix4f mat, Material *m1, Material *m2) ;
   
    virtual ~Checkerboard(){}
   
    virtual Vector3f getDiffuseColor() const {
        return diffuse_color;
    }
   
    Vector3f Shade(const Ray &ray, const Hit &hit, const Vector3f &dirToLight, 
    const Vector3f &lightColor) const;
       
    
   
protected:
    Matrix4f matrix;
    Material* mat1;
    Material* mat2;
};



#endif	/* CHECKERBOARD_H */

