

#include "Checkerboard.h"

Checkerboard::Checkerboard(Matrix4f mat, Material* m1, Material* m2): Material() 
{
    this->matrix = mat;
    this->mat1 = m1;
    this->mat2 = m2;
}

Vector3f Checkerboard::Shade(const Ray& ray, const Hit& hit, 
        const Vector3f& dirToLight, const Vector3f& lightColor) const
{
    Vector3f hp3 = ray.pointAtParameter(hit.getT());
    Vector4f hp4 (hp3[0], hp3[1], hp3[2], 1);
    Matrix4f mat = matrix;
    Vector4f hitPoint = mat*hp4;
    int xFloor = floor(hitPoint[0]);
    int yFloor = floor(hitPoint[1]);
    int zFloor = floor(hitPoint[2]);

    xFloor %= 2;
    yFloor %= 2;
    zFloor %= 2;

    xFloor += 2;
    yFloor += 2;
    zFloor += 2;

    xFloor %= 2;
    yFloor %= 2;
    zFloor %= 2;

    if ((xFloor^yFloor^zFloor) == 1) {
        return mat2->Shade(ray, hit, dirToLight, lightColor);
    }
    return mat1->Shade(ray, hit, dirToLight, lightColor);
}
