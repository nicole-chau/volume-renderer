#pragma once
#include "globals.h"
#include "ray.h"
#include <glm/glm.hpp>

class Camera
{
private:
    float fovy;
    unsigned int width, height;  // Screen dimensions
    float near_clip;  // Near clip plane distance
    float far_clip;  // Far clip plane distance

    //Computed attributes
    float aspect;

    glm::vec3 eye,      //The position of the camera in world space
    ref,      //The point in world space towards which the camera is pointing
    look,     //The normalized vector from eye to ref. Is also known as the camera's "forward" vector.
    up,       //The normalized vector pointing upwards IN CAMERA SPACE. This vector is perpendicular to LOOK and RIGHT.
    right,    //The normalized vector pointing rightwards IN CAMERA SPACE. It is perpendicular to UP and LOOK.
    world_up, //The normalized vector pointing upwards IN WORLD SPACE. This is primarily used for computing the camera's initial UP vector.
    V,        //Represents the vertical component of the plane of the viewing frustum that passes through the camera's reference point. Used in Camera::Raycast.
    H;        //Represents the horizontal component of the plane of the viewing frustum that passes through the camera's reference point. Used in Camera::Raycast.


    glm::mat4 rotate(float angle, float x, float y, float z);

    friend class RayCast;

public:
    Camera();
    Camera(unsigned int width, unsigned int height);
    Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp);
    Camera(const Camera &c);

    void recomputeAttributes();
//    void recomputePolarAttributes();

    glm::mat4 getViewProj();
    glm::mat4 getView();
    glm::mat4 getProj();


    void rotateAboutUp(float deg);
    void rotateAboutRight(float deg);

    void rotateTheta(float deg);
    void rotatePhi(float deg);

    void translateAlongLook(float amt);
    void translateAlongRight(float amt);
    void translateAlongUp(float amt);

    void zoom(float amt);

    void reset();

    Ray rayCast(const Point2f &pixel) const;         //Creates a ray in 3D space given a 2D point on the screen, in screen coordinates.
    Ray rayCast(float x, float y) const;            //Same as above, but takes two floats rather than a vec2.
    Ray rayCastNDC(float ndc_x, float ndc_y) const; //Creates a ray in 3D space given a 2D point in normalized device coordinates.

    friend class RayCast;
};
