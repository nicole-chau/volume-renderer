#include "camera.h"
#include "math.h"

Camera::Camera()
    : forward(0.f, 0.f, -1.f, 0.f),
      right(1.f, 0.f, 0.f, 0.f),
      up(0.f, 1.f, 0.f, 0.f),
      fov(45.f),
      eye(0.f, 0.f, 10.f, 1.f),
      ref(Point3f(0,0,0)),
      nearClip( 0.01),
      farClip(100.f),
      aspectRatio(1.f),
      width(400),
      height(400)
{
    float tanFov = tan(glm::radians(fov/2));
    float len = glm::length(ref - glm::vec3(eye.x, eye.y, eye.z));
    glm::vec4 v = (up * len * tanFov);
    vertical = Vector3f(v.x, v.y, v.z);

    glm::vec4 h = right * len * aspectRatio * tanFov;
    horizontal = Vector3f(h.x, h.y, h.z);
}

glm::mat4 Camera::viewMatrix() {
    glm::mat4 orientation(right, up, forward, glm::vec4(0.f, 0.f, 0.f, 1.f));
    glm::transpose(orientation);

    glm::mat4 translation(1.f);
    translation[3] = glm::vec4(-eye.x, -eye.y, -eye.z, eye.w);

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
    eye = rotate * eye;
}

void Camera::rotateRight(float deg) {
    glm::mat4 rotate = Camera::rotate(deg, 1.f, 0.f, 0.f);
    eye = rotate * eye;
}

void Camera::rotateUp(float deg) {
    glm::mat4 rotate = Camera::rotate(deg, 0.f, 1.f, 0.f);
    eye = rotate * eye;
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
Ray Camera::Raycast(const Point2f &pt) const
{
    return Raycast(pt.x, pt.y);
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
    Point3f eyeXYZ = Point3f(eye.x, eye.y, eye.z);
    Ray result(eyeXYZ, glm::normalize(P - eyeXYZ));
    return result;
}
