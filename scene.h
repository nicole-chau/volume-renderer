#pragma once
#include <QList>
//#include <raytracing/film.h>
#include "camera.h"
//#include <scene/lights/light.h>

class Primitive;
class Material;
class Light;

class Scene
{
public:
    Scene();
//    std::vector<uPtr<Primitive>> primitives;
//    std::vector<uPtr<Material>> materials;
//    std::vector<sPtr<Light>> lights;
    Camera camera;
//    Film film;

    void SetCamera(const Camera &c);

    void CreateTestScene();
    void Clear();

//    bool Intersect(const Ray& ray, Intersection* isect) const;
};
