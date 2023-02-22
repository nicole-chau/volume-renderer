#include "camera.h"
#include "math.h"

Camera::Camera()
    : forward(0.f, 0.f, -1.f, 0.f),
      right(1.f, 0.f, 0.f, 0.f),
      up(0.f, 1.f, 0.f, 0.f),
      fov(45.f),
      eye(0.f, 0.f, -10.f),
      ref(Point3f(0,0,0)),
      nearClip( 0.01),
      farClip(100.f),
      aspectRatio(1.f),
      width(32),
      height(32)
{
//    float tanFov = tan(glm::radians(fov/2));
//    float len = glm::length(ref - eye);
//    glm::vec4 v = (up * len * tanFov);
//    vertical = Vector3f(v.x, v.y, v.z);

//    glm::vec4 h = right * len * aspectRatio * tanFov;
//    horizontal = Vector3f(h.x, h.y, h.z);

    // RecomputeAttributes()
    Vector3f look = glm::normalize(ref - eye);
    Vector3f rightRecompute = glm::normalize(glm::cross(look, Vector3f(up.x, up.y, up.z)));
    Vector3f upRecompute = glm::cross(rightRecompute, look);
    right = glm::vec4(rightRecompute.x, rightRecompute.y, rightRecompute.z, 0.f);
    up = glm::vec4(upRecompute.x, upRecompute.y, upRecompute.z, 0.f);

    float tan_fovy = tan(glm::radians(fov/2));
    float len = glm::length(ref - eye);
    float aspect = width/(float)height;
    vertical = Vector3f(up)*len*tan_fovy;
    horizontal = Vector3f(right)*len*aspect*tan_fovy;
}

glm::mat4 Camera::viewMatrix() {
    glm::mat4 orientation(right, up, forward, glm::vec4(0.f, 0.f, 0.f, 1.f));
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

void Camera::translateForward(float z) {
    eye[2] += z;
}

void Camera::translateRight(float x) {
    eye[0] += x;
}

void Camera::translateUp(float y) {
    eye[1] += y;
}

void Camera::rotateForward(float deg) {
    glm::mat4 rotate = Camera::rotate(deg, 0.f, 0.f, 1.f);
    glm::vec4 result = rotate * glm::vec4(eye, 1.f);
    eye = glm::vec3(result.x, result.y, result.z);
}

void Camera::rotateRight(float deg) {
    glm::mat4 rotate = Camera::rotate(deg, 1.f, 0.f, 0.f);
    glm::vec4 result = rotate * glm::vec4(eye, 1.f);
    eye = glm::vec3(result.x, result.y, result.z);
}

void Camera::rotateUp(float deg) {
    glm::mat4 rotate = Camera::rotate(deg, 0.f, 1.f, 0.f);
    glm::vec4 result = rotate * glm::vec4(eye, 1.f);
    eye = glm::vec3(result.x, result.y, result.z);
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
Ray Camera::Raycast(const Point2f &pixel) const
{
   // return Raycast(pixel.x, pixel.y);

    // pixel to NDC
    Point2f ndc((2.f * pixel.x / width) - 1,
                1 - (2.f * pixel.y / height));

    // screen point to world point
    float alpha = glm::radians(fov / 2);
    float len = glm::length(ref - eye);
    Vector3f V = Vector3f(up.x, up.y, up.z) * len * tan(alpha);
    glm::vec3 right = glm::cross(glm::normalize(ref - eye), Vector3f(up.x, up.y, up.z));
    Vector3f H = right * len * aspectRatio * tan(alpha);
    Point3f pointW = ref + ndc.x * H + ndc.y * V;

    // world point to ray
    Point3f rayOrigin = eye;
    Vector3f rayDirection = glm::normalize(pointW - eye);
    return Ray(rayOrigin, rayDirection);
}

Ray Camera::Raycast(float x, float y) const
{
    float ndc_x = (2.f*x/width - 1);
    float ndc_y = (1 - 2.f*y/height);
    return RaycastNDC(ndc_x, ndc_y);
}

Ray Camera::RaycastNDC(float ndc_x, float ndc_y) const
{
    glm::vec3 P = ref + ndc_x * horizontal + ndc_y * vertical;
    Ray result(eye, glm::normalize(P - eye));
    return result;
}
