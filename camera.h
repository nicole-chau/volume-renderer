#pragma once
#include "globals.h"
#include "ray.h"
#include <glm/glm.hpp>

class Camera
{
private:
    glm::vec4 forward;
    glm::vec4 right;
    glm::vec4 up;
    float fov;
    Point3f eye;
    Point3f ref; // point in world space towards which camera is pointing
    float nearClip;
    float farClip;
    float aspectRatio;
    unsigned int width, height;  // Screen dimensions

    Vector3f vertical; //Represents the vertical component of the plane of the viewing frustum that passes through the camera's reference point. Used in Camera::Raycast.
    Vector3f horizontal; //Represents the horizontal component of the plane of the viewing frustum that passes through the camera's reference point. Used in Camera::Raycast.

    glm::mat4 rotate(float angle, float x, float y, float z);

public:
    Camera();

    glm::mat4 viewMatrix();
    glm::mat4 projMatrix();
    void translateForward(float z);
    void translateRight(float x);
    void translateUp(float y);
    void rotateForward(float deg);
    void rotateRight(float deg);
    void rotateUp(float deg);


    Ray Raycast(const Point2f &pixel) const;         //Creates a ray in 3D space given a 2D point on the screen, in screen coordinates.
    Ray Raycast(float x, float y) const;            //Same as above, but takes two floats rather than a vec2.
    Ray RaycastNDC(float ndc_x, float ndc_y) const; //Creates a ray in 3D space given a 2D point in normalized device coordinates.
};
