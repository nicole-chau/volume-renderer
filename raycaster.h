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

class RayCaster
{
public:
    RayCaster();
    QImage RenderData();
    void createPhantom();

private:
    // depth x height x width
    int phantom[32][32][32] = {{{}}};

    // Compute near and far intersections of ray with bounding box
    bool RayBoxIntersect(Ray ray, AABoundingBox box, float &tNear, float &tFar);
};

