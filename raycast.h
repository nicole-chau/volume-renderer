#pragma once

#include "globals.h"
#include "ray.h"
#include "camera.h"
#include <QImage>

struct AABoundingBox
{
    Point3f min;
    Point3f max;
    AABoundingBox(Point3f min, Point3f max)
        : min(min), max(max) {}
};

class RayCast
{
public:
    RayCast();
    QImage renderData();
    void createPhantom();

private:
    // width x height x depth (x, y, z)
    float phantom[32][32][32] = {{{}}};
    Camera camera;

    // Compute near and far intersections of ray with bounding box
    bool rayBoxIntersect(Ray ray, AABoundingBox box, float &tNear, float &tFar);

    // Sample voxel along ray
    Color3f sampleVolume(Ray ray, float tNear, float tFar);

    // Trilinear interpolation to get sampled value
    float trilinearInterp(Point3f pos);

    friend class MainWindow;
};

