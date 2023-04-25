 #pragma once

#include "colormap.h"
#include "globals.h"
#include "ray.h"
#include "camera.h"
#include <QImage>


// Min and max range of Houndsfield unit values in CT scan
const static float HU_MIN = -1024.f;
const static float HU_MAX = 3072.f;

struct BoundingBox
{
    Point3f min;
    Point3f max;
    glm::mat4 boxToWorld;
    glm::mat4 worldToBox;

    BoundingBox()
        : min(glm::vec4(0,0,0,1)), max(glm::vec4(0,0,0,1)),
          boxToWorld(glm::mat4()), worldToBox(glm::mat4())
    {}

    BoundingBox(glm::vec4 min, glm::vec4 max)
        : boxToWorld(glm::translate(glm::mat4(), glm::vec3(-max.x / 2.f, -max.y/2.f, -max.z/2.f))),
          worldToBox(glm::inverse(boxToWorld))
    {
        min = boxToWorld * min;
        this->min = Point3f(min.x, min.y, min.z);

        max = boxToWorld * max;
        this->max = Point3f(max.x, max.y, max.z);
    }
};

class RayCast
{
public:
    RayCast();
    RayCast(const std::vector<std::vector<double>>& data);
    RayCast(int width, int height, std::vector<std::vector<double>> data);

    void loadData(int width, int height, const std::vector<std::vector<double>>& data);

    void createCube();
    void createSphere();
    void createCubeVector();

    QImage renderData();

    void setUseRGB(bool useRGB);
    void setRGBMinMaxRange(int min, int max);

private:
    // width x height x depth (x, y, z)
    int cubeSize = 120;

    float phantom[120][120][120] = {{{}}};
    std::vector<std::vector<double>> phantomVector;

    int width;
    int height;
    int depth;

    std::vector<std::vector<double>> data;
    // Bounding box for data
    BoundingBox box;

    Camera camera;

    // RGB transfer function
    bool useRGB;
    int minHURange;
    int maxHURange;
    // Store maximum value for each interval within the range corresponding
    // to each color in the RGB color map
    std::vector<float> rangeIntervals;

    // Compute near and far intersections of ray with bounding box
    bool rayBoxIntersect(Ray ray, float &tNear, float &tFar);

    // Sample voxel along ray
    Color3f sampleVolume(Ray ray, float tNear, float tFar);

    // Grid marching
    void gridMarch(glm::vec3 rayOrigin, glm::vec3 rayDirection, float maxLength, float *out_dist, glm::vec3 *out_cellsHit);

    // Trilinear interpolation to get sampled value
    float trilinearInterp(Point3f pos);
    float clamp(float x, float min, float max);

    void getRGBColor(float hounsfield, float density, Color3f* color);

    friend class MainWindow;
};

