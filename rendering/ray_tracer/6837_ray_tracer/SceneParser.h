#ifndef SCENE_PARSER_H
#define SCENE_PARSER_H

#include <cassert>
#include <vecmath.h>

#include "Light.h"
#include "Material.h"
#include "Group.h"

class BoundingSphere;
class Camera;
class Light;
class Material;
class Object3D;
class Group;
class Sphere;
class Plane;
class Triangle;
class Transform;

#define MAX_PARSER_TOKEN_LENGTH 100

class SceneParser
{
public:

    SceneParser( const char* filename );
    ~SceneParser();

    Camera* getCamera() const
    {
        return camera;
    }

    Vector3f getBackgroundColor() const
    {
        return background_color;
    }

    Vector3f getAmbientLight() const
    {
        return ambient_light;
    }
    
    int getNumLights() const
    {
        return num_lights;
    }
    
    Light* getLight( int i ) const
    {
        assert( i >= 0 && i < num_lights );
        return lights[i];
    }

    int getNumMaterials() const
    {
        return num_materials;
    }

    Material* getMaterial( int i ) const
    {
        assert( i >= 0 && i < num_materials );
        return materials[i];
    }

    Group* getGroup() const
    {
        return group;
    }

private:

    SceneParser()
    {
        assert( false );
    }

    void parseFile();
    void parseOrthographicCamera();
    void parsePerspectiveCamera();
    void parseBackground();
    void parseLights();
    Light* parseDirectionalLight();
    Light* parsePointLight();
    void parseMaterials();
    Material* parsePhongMaterial();
    Material* parseCheckerboard( int count );
    Material* parseNoise( int count );
    Material* parseMarble( int count );
    Material* parseWood( int count );
    // jiawen: removed so code compiles
    // Material* parseWood( int count );

    Object3D* parseObject( char token[ MAX_PARSER_TOKEN_LENGTH ] );
    Group* parseGroup();
    Sphere* parseSphere();
    Plane* parsePlane();
    Triangle* parseTriangle();
    Group* parseTriangleMesh();
    Transform* parseTransform();
    BoundingSphere* parseBoundingSphere();
    void parseMatrixHelper( Matrix4f& matrix, char token[ MAX_PARSER_TOKEN_LENGTH ] );

    int getToken( char token[ MAX_PARSER_TOKEN_LENGTH ] );
    Vector3f readVector3f();
    Vector2f readVector2f();
    float readFloat();
    int readInt();

    FILE* file;
    Camera* camera;
    Vector3f background_color;
    Vector3f ambient_light;
    int num_lights;
    Light** lights;
    int num_materials;
    Material** materials;
    Material* current_material;
    Group* group;
};

#endif // SCENE_PARSER_H