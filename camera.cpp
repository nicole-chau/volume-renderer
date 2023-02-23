#include "camera.h"
#include "math.h"

Camera::Camera()
    : Camera(400, 400)
{
    forward = glm::vec3(0,0,-1);
    up = glm::vec3(0,1,0);
    right = glm::vec3(1,0,0);

    phi = 0.f;
    theta = 0.f;
    r = 10.f;
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp)
    : worldUp(worldUp),
      fov(45),
      eye(e),
      ref(r),
      nearClip(0.1f),
      farClip(1000),
      width(w),
      height(h),
      phi(0.f), theta(0.f), r(10.f)
{
    recomputeAttributes();
//    recomputePolarAttributes();
}

void Camera::recomputeAttributes()
{
    forward = glm::normalize(ref - eye);
    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::cross(right, forward);

    float tan_fovy = tan(glm::radians(fov/2));
    float len = glm::length(ref - eye);
    float aspect = width/(float)height;
    vertical = up*len*tan_fovy;
    horizontal = Vector3f(right.x, right.y, right.z)*len*aspect*tan_fovy;
}

void Camera::recomputePolarAttributes() {
    glm::vec4 unitEye(0.f, 0.f, 0.f, 1.f);
    glm::vec4 unitForward(0.f, 0.f, 1.f, 0.f);
    glm::vec4 unitUp(0.f, 1.f, 0.f, 0.f);
    glm::vec4 unitRight(1.f, 0.f, 0.f, 0.f);

    glm::mat4 polarMat(1.f);

    polarMat = glm::rotate(polarMat, glm::radians(theta), glm::vec3(0.f, 1.f, 0.f))
               * glm::rotate(polarMat, glm::radians(phi), glm::vec3(1.f, 0.f, 0.f))
               * glm::translate(polarMat, glm::vec3(0.f, 0.f, r));

    eye = glm::vec3(polarMat * unitEye);
    forward = glm::vec3(polarMat * unitForward);
    up = glm::vec3(polarMat * unitUp);
    right = glm::vec3(polarMat * unitRight);
}

glm::mat4 Camera::getViewProj()
{
    glm::mat4 rot = glm::rotate(glm::mat4(), glm::radians(theta), glm::vec3(0,1,0)) * glm::rotate(glm::mat4(), glm::radians(phi), glm::vec3(1,0,0));
    glm::vec3 eye = glm::vec3(rot * glm::vec4(0, 0, 1.5f, 1));
    glm::vec3 up = glm::vec3(rot * glm::vec4(0,1,0,0));
    return glm::perspective(glm::radians(45.f), width/(float)height, 0.01f, 10.f) * glm::lookAt(eye, glm::vec3(0,0,0), up);
//    return glm::perspective(glm::radians(fov), width / (float)height, nearClip, farClip) * glm::lookAt(eye, ref, up);
}

/*
glm::mat4 Camera::viewMatrix() {
    glm::mat4 orientation(glm::vec4(right, 1.f), glm::vec4(up, 1.f), glm::vec4(forward, 1.f), glm::vec4(0.f, 0.f, 0.f, 1.f));
    glm::transpose(orientation);

    glm::mat4 translation(1.f);
    translation[3] = glm::vec4(-eye.x, -eye.y, -eye.z, 1.f);

    return orientation * translation;
}


glm::mat4 Camera::projMatrix() {
    glm::mat4 result;

    result[0][0] = 1.f / (aspectRatio * tanf(fov / 2.f));
    result[1][1] = 1.f / (tanf(fov / 2.f));
    result[2][2] = farClip / (farClip - nearClip);
    result[3][2] = (-farClip * nearClip) / (farClip - nearClip);
    result[2][3] = 1.f;

    return result;
}
*/

void Camera::translateForward(float z) {
//    eye[2] += z;
//    r += z;
//    recomputePolarAttributes();

    glm::vec3 translation = forward * z;
    eye += translation;
    ref += translation;
}

void Camera::translateRight(float x) {
//    eye[0] += x;
    glm::vec3 translation = right * x;
    eye += translation;
    ref += translation;
}

void Camera::translateUp(float y) {
//    eye[1] += y;
    glm::vec3 translation = up * y;
    eye += translation;
    ref += translation;
}

void Camera::rotateForward(float deg) {
    glm::mat4 rotate = Camera::rotate(deg, 0.f, 0.f, 1.f);
    glm::vec4 result = rotate * glm::vec4(eye, 1.f);
    eye = glm::vec3(result.x, result.y, result.z);

}

void Camera::rotateRight(float deg) {
//    glm::mat4 rotate = Camera::rotate(deg, 1.f, 0.f, 0.f);
//    glm::vec4 result = rotate * glm::vec4(eye, 1.f);
//    eye = glm::vec3(result.x, result.y, result.z);

    phi += deg;
    recomputePolarAttributes();

//    deg = glm::radians(deg);
//    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, right);
//    ref = ref - eye;
//    ref = glm::vec3(rotation * glm::vec4(ref, 1));
//    ref = ref + eye;
//    recomputeAttributes();
}

void Camera::rotateUp(float deg) {
//    glm::mat4 rotate = Camera::rotate(deg, 0.f, 1.f, 0.f);
//    glm::vec4 result = rotate * glm::vec4(eye, 1.f);
//    eye = glm::vec3(result.x, result.y, result.z);

    theta += deg;
    recomputePolarAttributes();

//    deg = glm::radians(deg);
//    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, up);
//    ref = ref - eye;
//    ref = glm::vec3(rotation * glm::vec4(ref, 1));
//    ref = ref + eye;
//    recomputeAttributes();
}


glm::mat4 Camera::rotate(float angle, float x, float y, float z) {
    if (x == 0.f && y == 0.f && z == 0.f) {
        glm::vec4 undefined = glm::vec4(std::numeric_limits<double>::quiet_NaN(),
                              std::numeric_limits<double>::quiet_NaN(),
                              std::numeric_limits<double>::quiet_NaN(),
                              std::numeric_limits<double>::quiet_NaN());
        return glm::mat4(undefined, undefined, undefined, undefined);
    } else {
        glm::mat4 result;

        glm::vec4 axis = glm::vec4(x, y, z, 0.f);
        axis = glm::normalize(axis);

        float angleRadian = angle * M_PI / 180;
        float cosAngle = cos(angleRadian);
        float sinAngle = sin(angleRadian);

        float xy = axis[0] * axis[1];
        float xz = axis[0] * axis[2];
        float yz = axis[1] * axis[2];

        for (int i = 0; i < 3; ++i) {
            result[i][i] = cosAngle + pow(axis[i], 2) * (1 - cosAngle);
        }

        result[1][0] = -axis[2] * sinAngle + xy * (1 - cosAngle);
        result[2][0] = axis[1] * sinAngle + xz * (1 - cosAngle);
        result[0][1] = axis[2] * sinAngle + xy * (1 - cosAngle);
        result[2][1] = -axis[0] * sinAngle + yz * (1 - cosAngle);
        result[0][2] = -axis[1] * sinAngle + xz * (1 - cosAngle);
        result[1][2] = axis[0] * sinAngle + yz * (1 - cosAngle);

        return result;
    }
}

// RAY CASTING
Ray Camera::rayCast(const Point2f &pixel) const
{
    // pixel to NDC
    Point2f ndc((2.f * pixel.x / width) - 1,
                1 - (2.f * pixel.y / height));

    glm::vec3 P = ref + ndc.x * horizontal + ndc.y * vertical;
    Ray result(eye, glm::normalize(P - eye));
    return result;

    // screen point to world point
//    float alpha = glm::radians(fov / 2);
//    float len = glm::length(ref - eye);
//    Vector3f V = Vector3f(up.x, up.y, up.z) * len * tan(alpha);
//    glm::vec3 right = glm::cross(glm::normalize(ref - eye), Vector3f(up.x, up.y, up.z));
//    Vector3f H = right * len * aspectRatio * tan(alpha);
//    Point3f pointW = ref + ndc.x * H + ndc.y * V;

//    // world point to ray
//    Point3f rayOrigin = eye;
//    Vector3f rayDirection = glm::normalize(pointW - eye);
//    return Ray(rayOrigin, rayDirection);
}

/*
Ray Camera::rayCast(float x, float y) const
{
    float ndc_x = (2.f*x/width - 1);
    float ndc_y = (1 - 2.f*y/height);
    return RaycastNDC(ndc_x, ndc_y);
}

Ray Camera::rayCastNDC(float ndc_x, float ndc_y) const
{
    glm::vec3 P = ref + ndc_x * horizontal + ndc_y * vertical;
    Ray result(eye, glm::normalize(P - eye));
    return result;
}
*/
