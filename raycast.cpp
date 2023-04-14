#include "raycast.h"

#include "math.h"
#include "colormap.h"

RayCast::RayCast()
    : width(512),
      height(512),
      depth(512),
      data({}),
      camera(512, 512),
      useRGB(false),
      minHURange(-1024),
      maxHURange(3072),
      rangeIntervals({})
{}

void RayCast::loadData(int width, int height, const std::vector<std::vector<double>>& data)
{
    // Load data and data size properties
    this->width = width;
    this->height = height;
    this->data = data;
    this->depth = data.size();


    // Initialize bounding box
    glm::vec4 boxMin = glm::vec4(0.f ,0.f ,0.f ,1.f);
    glm::vec4 boxMax = glm::vec4(width, height, depth, 1.f);
    box = BoundingBox(boxMin, boxMax);

//    this->camera.reset();
//    this->camera.ref = glm::vec3(boxMin + boxMax) * 0.5f;
}

void RayCast::createCubeVector()
{
    int min = cubeSize * 0.2;
    int max = cubeSize * 0.8;

    for (int i = 0; i < cubeSize; ++i)
    {
        std::vector<double> slice;

        for (int j = 0; j < cubeSize * cubeSize; ++j)
        {
            // (x, y) -> [x + W * y]
            if ((j <= (cubeSize * min) || j >= (cubeSize * max + 1)) && (i <= min || i > max)){
                slice.push_back(0.1);
            } else if ((j % cubeSize < min || j % cubeSize > max) && (i <= min || i < max)) {
                slice.push_back(0.1);
            } else {
                slice.push_back(0.5);
            }
        }

        phantomVector.push_back(slice);
    }

    data = phantomVector;
    width = cubeSize;
    height = cubeSize;
    depth = cubeSize;

    glm::vec4 boxMin = glm::vec4(0.f, 0.f, 0.f, 1.f);
    glm::vec4 boxMax = glm::vec4 (width, height, depth, 1.f);
    box = BoundingBox(boxMin, boxMax);

}

// Compute near and far intersections of ray with bounding box
bool RayCast::rayBoxIntersect(Ray ray, float &tNear, float &tFar) {
    Point3f tMin = (box.min - ray.origin) / ray.direction;
    Point3f tMax = (box.max - ray.origin) / ray.direction;
    Point3f t1 = glm::min(tMin, tMax);
    Point3f t2 = glm::max(tMin, tMax);
    tNear = std::max(std::max(t1.x, t1.y), t1.z);
    tFar = std::min(std::min(t2.x, t2.y), t2.z);

    // Intersection if tFar > tNear
    return tFar > tNear;
}

void RayCast::gridMarch(glm::vec3 rayOrigin, glm::vec3 rayDirection, float maxLength, float *out_dist, glm::vec3 *out_currPos) {
    glm::ivec3 currCell = glm::ivec3(glm::floor(rayOrigin));
    rayDirection = glm::normalize(rayDirection); // Now all t values represent world dist.

    float min_t = glm::sqrt(3.f);
    float interfaceAxis = -1; // Track axis for which t is smallest
    for (int i = 0; i < 3; ++i) { // Iterate over the three axes
        if (rayDirection[i] != 0) { // Is ray parallel to axis i?
            float offset = glm::max(0.f, glm::sign(rayDirection[i])); // See slide 5
            // If the player is *exactly* on an interface then
            // they'll never move if they're looking in a negative direction
            if (currCell[i] == rayOrigin[i] && offset == 0.f) {
                offset = -1.f;
            }
            int nextIntercept = currCell[i] + offset;
            float axis_t = (nextIntercept - rayOrigin[i]) / rayDirection[i];
            axis_t = glm::min(axis_t, maxLength); // Clamp to max len to avoid super out of bounds errors
            if(axis_t < min_t) {
                min_t = axis_t;
                interfaceAxis = i;
            }
        }
    }

    if(interfaceAxis == -1) {
        throw std::out_of_range("interfaceAxis was -1 after the for loop in gridMarch!");
    }

    rayOrigin += rayDirection * min_t;
    glm::ivec3 offset = glm::ivec3(0,0,0);
    // Sets it to 0 if sign is +, -1 if sign is -
    offset[interfaceAxis] = glm::min(0.f, glm::sign(rayDirection[interfaceAxis]));

    currCell = glm::ivec3(glm::floor(rayOrigin)) + offset;

    *out_currPos = currCell;
}

Color3f RayCast::sampleVolume(Ray ray, float tNear, float tFar) {
    float stepSize = 1.f;
    float k = useRGB ? 0.9 : 0.5;

    Point3f start = ray.origin + (ray.direction * tNear);
    Point3f end = ray.origin + (ray.direction * tFar);

    glm::vec3 currPos = start;

    // Initialize transmittance
    Color3f color(0.f);
    float transmittance = 1; // 1 if clear, 0 if opaque

    // Loop while we have not reached end of box
    float currLength = glm::distance(currPos, start);
    float endLength = glm::distance(end, start);

    int numSteps = 0;
    float prevDensity;
    while (currLength < endLength)
    {
        // Grid march
        float distance;
        gridMarch(currPos, ray.direction, glm::length(ray.direction * tFar), &distance, &currPos);
        currLength = glm::distance(currPos, start);

        if (transmittance >= 0) // something is still visible
        {
            // Transform currPos from world space back to object space to access voxel data
            glm::vec4 dataPos = box.worldToBox * glm::vec4(currPos, 1.f);



            if (dataPos.x < width && dataPos.y < height && dataPos.z < depth) {
//                float density = trilinearInterp(Point3f(dataPos.x, dataPos.y, dataPos.z));

                // Flip x and y values because data is loaded in in this order
                float hounsfield = data.at(dataPos.z).at(dataPos.y * width + dataPos.x);

                hounsfield = std::min(std::max(HU_MIN, hounsfield), HU_MAX);

                // Map HU range [-1024, 3072] to [0, 1]
                float density = hounsfield;
                density = (density + -HU_MIN) / (HU_MAX - HU_MIN);

                Color3f currColor;

                if (useRGB) {
                    if (abs(density - prevDensity) < 0.01) {
                        currColor = Color3f(0.f);
                    } else {
                        getRGBColor(hounsfield, density, &currColor);
                    }
                } else {
                    currColor = Color3f(density);
                }

                // Process voxel value
                transmittance *= exp(k * -stepSize * density);
                color += stepSize * density * currColor * transmittance;

                if (dataPos.x == 315 && dataPos.y == 320) {
                    if(useRGB) {
                        getRGBColor(hounsfield, density, &currColor);
                    }
                    qDebug() << currColor.x << ", " << currColor.y << ", " << currColor.z;
                    qDebug() << "Density: " << density;
                }

                numSteps++;
                prevDensity = density;
            }

        }

    }

    color.r = clamp(color.r, 0, 1);
    color.g = clamp(color.g, 0, 1);
    color.b = clamp(color.b, 0, 1);

//    if (useRGB) {
//        color /= log2(numSteps);
//    }

    return color;
}

float getTValue(float min, float max, float x) {
    return ((x - min)) / (max - min);
}

Substance RayCast::getSubstanceType(float hounsfield, float *subMin, float *subMax) {
    if (hounsfield >= AIR_MIN && hounsfield < AIR_MAX) {
        *subMin = AIR_MIN;
        *subMax = AIR_MAX;
//        return AIR;
    } else if (hounsfield >= LUNG_MIN && hounsfield < LUNG_MAX) {
        *subMin = LUNG_MIN;
        *subMax = LUNG_MAX;
        return LUNG;
    } else if (hounsfield >= BONE_MIN && hounsfield < BONE_MAX) {
        *subMin = BONE_MIN;
        *subMax = BONE_MAX;
        return BONE;
    }

    return UNKNOWN;
}

void RayCast::getGrayscaleColor(float hounsfield, Color3f *color, float *density) {

    float subMin;
    float subMax;
    Substance sub = getSubstanceType(hounsfield, &subMin, &subMax);

    if (sub == UNKNOWN) {
        *density = hounsfield;
        return;
    }

    float t = getTValue(subMin, subMax, hounsfield);
    std::tuple colors = grayscale.find(sub)->second;

    float grayscaleVal = glm::mix(std::get<0>(colors), std::get<1>(colors), t);

    *color = Color3f(grayscaleVal);
//    *density = glm::mix(subMin, subMax, t);
    *density = hounsfield;

}

void RayCast::setUseRGB(bool useRGB) {
    this->useRGB = useRGB;
}

void RayCast::setRGBMinMaxRange(int min, int max)
{
    minHURange = min;
    maxHURange = max;
    float rangeDiff = maxHURange - minHURange;

    // Define maximum value for each interval within the range corresponding
    // to each color in the RGB color map
    rangeIntervals.clear();
    for (int i = 0; i < 5; ++i) {
        float intervalMax = minHURange + i * (rangeDiff * 0.25);
        rangeIntervals.push_back(intervalMax);
    }
}

void RayCast::getRGBColor(float hounsfield, float density, Color3f* color) {
    if (hounsfield >= minHURange && hounsfield <= maxHURange) {
        if (hounsfield <= rangeIntervals[1]) {
            float t = getTValue(rangeIntervals[0], rangeIntervals[1], hounsfield);
            *color = glm::mix(colorScale[0], colorScale[1], t);

        } else if (hounsfield <= rangeIntervals[2]) {
            float t = getTValue(rangeIntervals[1] + 1, rangeIntervals[2], hounsfield);
            *color = glm::mix(colorScale[1], colorScale[2], t);

        } else if (hounsfield <= rangeIntervals[3]) {
            float t = getTValue(rangeIntervals[2] + 1, rangeIntervals[3], hounsfield);
            *color = glm::mix(colorScale[2], colorScale[3], t);

        } else {
            float t = getTValue(rangeIntervals[3] + 1, rangeIntervals[4], hounsfield);
            *color = glm::mix(colorScale[3], colorScale[4], t);
        }

//        *color = *color / 255.f;

    } else {
        *color = Color3f(density);
    }
}


float RayCast::trilinearInterp(Point3f pos)
{
    // TODO: Map to voxel data indices
    // (0, 0, 20) --> (16, 16, 0)
    float x = pos.x + width / 2;
    float y = pos.y + height / 2;
    float z = pos.z - 100;

    // Clamp to index bounds
    int xf = clamp(std::floor(x), 0, width - 1);
    int yf = clamp(std::floor(y), 0, height - 1);
    int zf = clamp(std::floor(z), 0, depth - 1);

    int xc = clamp(std::ceil(x), 0, width - 1);
    int yc = clamp(std::ceil(y), 0, height - 1);
    int zc = clamp(std::ceil(z), 0, depth - 1);

    // Fractional part of considered resampling location's position
    float xfrac = x - std::floor(x);
    float yfrac = y - std::floor(y);
    float zfrac = z - std::floor(z);

    float result = data.at(zf).at(xf + width * yf) * (1 - xfrac) * (1 - yfrac) * (1 - zfrac)
            + data.at(zf).at(xc + width * yf) * xfrac * (1 - yfrac) * (1 - zfrac)
            + data.at(zf).at(xf + width * yc) * (1 - xfrac) * yfrac * (1 - zfrac)
            + data.at(zc).at(xf + width * yf) * (1 - xfrac) * (1 - yfrac) * zfrac
            + data.at(zc).at(xc + width * yf) * xfrac * (1 - yfrac) * zfrac
            + data.at(zc).at(xf + width * yc) * (1 - xfrac) * yfrac * zfrac
            + data.at(zf).at(xc + width * yc) * xfrac * yfrac * (1 - zfrac)
            + data.at(zc).at(xc + width * yc) * xfrac * yfrac * zfrac;

    return result;
}

float RayCast::clamp(float x, float min, float max)
{
    return std::min(max, std::max(min, x));
}

QImage RayCast::renderData()
{
    QImage result(512, 512, QImage::Format_RGB32);
//    result = result.scaled(512, 512, Qt::KeepAspectRatio);
    result.fill(qRgb(0.f, 0.f, 0.f));


    // ----------------------------------------------------------------------//
    // ----------------------- RAY CASTING ALGORITHM ----------------------- //
    // ----------------------------------------------------------------------//

    // Iterate through each pixel
    for (int w = 0; w < result.width(); ++w) {
        for (int h = 0; h < result.height(); ++h) {
            // Initialize pixel color to black
            Color3f color(0, 0, 0);

            // Cast ray to pixel on screen
            Point2f pixel(w, h);
            Ray ray = camera.rayCast(pixel);

//            color = ray.direction;

            // Set tNear = -infinity and tFar = infinity
            float tNear = -std::numeric_limits<double>::infinity();
            float tFar = std::numeric_limits<double>::infinity();

            // Check intersection of ray with axis aligned bounding box
            bool hasIntersect = rayBoxIntersect(ray, tNear, tFar);

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
