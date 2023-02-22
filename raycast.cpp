#include "raycast.h"

RayCast::RayCast()
    : camera(Camera())
{
    createPhantom();
}

// Phantom size should just correspond to pixel locations
// bottom left front corner (-16,-16, 20)
void RayCast::createPhantom()
{
    // Create phantom cube for testing
    int depth = 32;
    int height = 32;
    int width = 32;

    int min = 8;
    int max = 24;

    for (int d = 0; d < depth; ++d) {
        for (int h = 0; h < height; ++h) {
            for (int w = 0; w < width; ++w) {
                if ((d >= min && h >= min && w >= min)
                    && (d <= max && h <= max && w <= max))
                {
                    // set density to 0.2
                    phantom[w][h][d] = 1;
                } else
                {
                    phantom[w][h][d] = 0;
                }

            }
        }
    }
}


// Compute near and far intersections of ray with bounding box
bool RayCast::rayBoxIntersect(Ray ray, AABoundingBox box, float &tNear, float &tFar) {
    /*
    // X Plane
    float xo = ray.origin.x;
    float xd = ray.direction.x;
    if (xd != 0)
    {
        float txMin = (box.min.x - xo) / xd;
        float txMax = (box.max.x - xo) / xd;
        if (txMin > txMax)
        {
            std::swap(txMin, txMax);
        }
        tNear = txMin > tNear ? txMin : tNear;
        tFar = txMax < tFar ? txMax : tFar;
    }

    // Y Plane
    float yo = ray.origin.y;
    float yd = ray.direction.y;
    if (yd != 0)
    {
        float tyMin = (box.min.y - yo) / yd;
        float tyMax = (box.max.y - yo) / yd;
        if (tyMin > tyMax)
        {
            std::swap(tyMin, tyMax);
        }
        tNear = tyMin > tNear ? tyMin : tNear;
        tFar = tyMax < tFar ? tyMax : tFar;
    }

    // Z Plane
    float zo = ray.origin.z;
    float zd = ray.direction.z;
    if (zd != 0)
    {
        float tzMin = (box.min.z - zo) / zd;
        float tzMax = (box.max.z - zo) / zd;
        if (tzMin > tzMax)
        {
            std::swap(tzMin, tzMax);
        }
        tNear = tzMin > tNear ? tzMin : tNear;
        tFar = tzMax < tFar ? tzMax : tFar;
    }
    */

    Point3f tMin = (box.min - ray.origin) / ray.direction;
    Point3f tMax = (box.max - ray.origin) / ray.direction;
    Point3f t1 = glm::min(tMin, tMax);
    Point3f t2 = glm::max(tMin, tMax);
    tNear = std::max(std::max(t1.x, t1.y), t1.z);
    tFar = std::min(std::min(t2.x, t2.y), t2.z);

    // Intersection if tFar > tNear
    return tFar > tNear;
}

Color3f RayCast::sampleVolume(Ray ray, float tNear, float tFar) {
    float stepSize = 10; // TODO: update

    Point3f start = ray.origin + (ray.direction * tNear);
    Point3f end = ray.origin + (ray.direction * tFar);

    Point3f currPos = start;

    // Initialize transmittance
    float transmittance = 1; // 1 if clear, 0 if opaque

    // Loop while we have not reached end of box
    float currLength = glm::distance(currPos, start);
    float endLength = glm::distance(end, start);
    while (currLength < endLength)
    {
        if (transmittance >= 0) // something is still visible
        {
            // Trilinearly interpolate voxel value at currPos
            float sampledVal = trilinearInterp(currPos);

            if (sampledVal > 0) {
                std::string x = "hi";
            }

            // Process voxel value
            // TODO: update transmittance calculation
            transmittance -= sampledVal;
        }

        // Step along ray
        currPos = currPos + (ray.direction * stepSize);
        currLength = glm::distance(currPos, start);
    }

    Color3f color(transmittance);
    return color;
}

float RayCast::trilinearInterp(Point3f pos)
{
    float x = pos.x + 16;
    float y = pos.y + 16;
    float z = pos.z - 20;

    int xf = std::floor(x);
    int yf = std::floor(y);
    int zf = std::floor(z);

    return phantom[xf][yf][zf];

    int xc = std::ceil(x);
    int yc = std::ceil(y);
    int zc = std::ceil(z);

    // TODO: map world coords back to voxel indices
    // (0, 0, 20) --> (16, 16, 0)

    // Fractional part of considered resampling location's position
    float xfrac = x - std::floor(x);
    float yfrac = y - std::floor(y);
    float zfrac = z - std::floor(z);

    float result = phantom[xf][yf][zf] * (1 - xfrac) * (1 - yfrac) * (1 - zfrac)
            + phantom[xc][yf][zf] * xfrac * (1 - yfrac) * (1 - zfrac)
            + phantom[xf][yc][zf] * (1 - xfrac) * yfrac * (1 - zfrac)
            + phantom[xf][yf][zc] * (1 - xfrac) * (1 - yfrac) * zfrac
            + phantom[xc][yf][zc] * xfrac * (1 - yfrac) * zfrac
            + phantom[xf][yc][zc] * (1 - xfrac) * yfrac * zfrac
            + phantom[xc][yc][zf] * xfrac * yfrac * (1 - zfrac)
            + phantom[xc][yc][zc] * xfrac * yfrac * zfrac;

    return result;
}


QImage RayCast::renderData()
{
    QImage result(32, 32, QImage::Format_RGB32);
//    result = result.scaled(512, 512, Qt::KeepAspectRatio);
    result.fill(qRgb(0.f, 0.f, 0.f));


    // ------------ RAY CASTING ALGORITHM ------------
    // Initialize bounding box -- size of phantom voxel data
    AABoundingBox box(Point3f(-16,-16,20), Point3f(16,16,52));

    // Iterate through each pixel
    for (int w = 0; w < result.width(); ++w) {
        for (int h = 0; h < result.height(); ++h) {

            if (w == 31 && h == 31) {
                int x = 0;
            }

            // Initialize pixel color to black
            Color3f color(0, 0, 0);

            // Cast ray to pixel on screen
            Point2f pixel(w, h);
            Ray ray = camera.Raycast(pixel);

            // Set tNear = -infinity and tFar = infinity
            float tNear = -std::numeric_limits<double>::infinity();
            float tFar = std::numeric_limits<double>::infinity();

            // Check intersection of ray with axis aligned bounding box
            bool hasIntersect = rayBoxIntersect(ray, box, tNear, tFar);

            // Sample volume along ray
            if (hasIntersect)
            {
                color = sampleVolume(ray, tNear, tFar);
            }

            // Set pixel color
            color *= 255;
            result.setPixelColor(w, h, qRgb(color.r, color.g, color.b));
        }
    }
    return result;
}
