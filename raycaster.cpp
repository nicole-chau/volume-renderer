#include "raycaster.h"

RayCaster::RayCaster()
{

}

void RayCaster::createPhantom()
{
    // Create phantom cube for testing
    int depth = sizeof(phantom) / sizeof(int);
    int height = sizeof(phantom[0]) / sizeof(int);
    int width = sizeof(phantom[0][0]) / sizeof(int);

    int min = -8;
    int max = 8;

    for (int d = 0; d < depth; ++d) {
        for (int h = 0; h < height; ++h) {
            for (int w = 0; w < width; ++w) {
                if ((d >= min && h >= min && w >= min)
                    || (d <= max && h <= max && w <= max))
                {
                    // set density to 1
                    phantom[d][h][w] = 1;
                } else
                {
                    phantom[d][h][w] = 0;
                }

            }
        }
    }
}


// Compute near and far intersections of ray with bounding box
bool RayCaster::RayBoxIntersect(Ray ray, AABoundingBox box, float &tNear, float &tFar) {
    Point3f tMin = (box.min - ray.origin) / ray.direction;
    Point3f tMax = (box.max - ray.origin) / ray.direction;
    Point3f t1 = glm::min(tMin, tMax);
    Point3f t2 = glm::max(tMin, tMax);
    tNear = std::max(std::max(t1.x, t1.y), t1.z);
    tFar = std::min(std::min(t2.x, t2.y), t2.z);

    // Intersection if tFar > tNear
    return tFar > tNear;
}

QImage RayCaster::RenderData()
{
    QImage result(32, 32, QImage::Format_RGB32);
    result = result.scaled(512, 512, Qt::KeepAspectRatio);
    result.fill(qRgb(0.f, 0.f, 0.f));


    // ------------ RAY CASTING ALGORITHM ------------
    // Initialize camera and bounding box
    Camera camera;
    AABoundingBox box(Point3f(0,0,0), Point3f(32, 32, 32));

    // Cast ray
    Point2f pixel(10, 10);
    Ray ray = camera.Raycast(pixel);

    // Check intersection with axis aligned bounding box



    return result;
}
