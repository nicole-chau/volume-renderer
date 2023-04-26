#include "camera.h"
#include "math.h"

#include <QDebug>

Camera::Camera():
    Camera(512, 512)
{
    look = glm::vec3(0,0,-1);
    up = glm::vec3(0,1,0);
    right = glm::vec3(1,0,0);
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(0,0,400), glm::vec3(0,0,0), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(55),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    eye(e),
    ref(r),
    world_up(worldUp)
{
    recomputeAttributes();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(c.eye),
    ref(c.ref),
    look(c.look),
    up(c.up),
    right(c.right),
    world_up(c.world_up),
    V(c.V),
    H(c.H)
{}


void Camera::recomputeAttributes()
{
    look = glm::normalize(ref - eye);
    right = glm::normalize(glm::cross(look, world_up));
    up = glm::cross(right, look);

    float tan_fovy = glm::tan(fovy/2);
    float len = glm::length(ref - eye);
    aspect = width/height;
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(fovy, width / (float)height, near_clip, far_clip) * glm::lookAt(eye, ref, up);
}

glm::mat4 Camera::getView()
{
    return glm::lookAt(eye, ref, up);
}

glm::mat4 Camera::getProj()
{
    return glm::perspective(fovy, width / (float)height, near_clip, far_clip);
}

void Camera::reset()
{
    fovy = 55.f;
    eye = glm::vec3(0, 0, 400);
    ref = glm::vec3(0, 0, 0);
    world_up = glm::vec3(0, 1, 0);
    recomputeAttributes();
}

void Camera::rotateAboutUp(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, up);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    recomputeAttributes();
}
void Camera::rotateAboutRight(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, right);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    recomputeAttributes();
}

void Camera::rotateTheta(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, right);
    eye = eye - ref;
    eye = glm::vec3(rotation * glm::vec4(eye, 1.f));
    eye = eye + ref;
    recomputeAttributes();
}

void Camera::rotatePhi(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, up);
    eye = eye - ref;
    eye = glm::vec3(rotation * glm::vec4(eye, 1.f));
    eye = eye + ref;
    recomputeAttributes();
}

void Camera::translateAlongLook(float amt)
{
    glm::vec3 translation = look * amt;
    eye += translation;
    ref += translation;
}

void Camera::translateAlongRight(float amt)
{
    glm::vec3 translation = right * amt;
    eye += translation;
    ref += translation;
}

void Camera::translateAlongUp(float amt)
{
    glm::vec3 translation = up * amt;
    eye += translation;
    ref += translation;
}

void Camera::zoom(float amt)
{
    glm::vec3 translation = look * amt;
    eye += translation;
}

// RAY CASTING
Ray Camera::rayCast(const Point2f &pixel) const
{
    // pixel to NDC
    Point2f ndc((2.f * pixel.x / width) - 1,
                1 - (2.f * pixel.y / height));

    glm::vec3 P = ref + ndc.x * H + ndc.y * V;
//    Ray result(eye, glm::normalize(ref));
    Ray result(eye, glm::normalize(P - eye));
    return result;
}
