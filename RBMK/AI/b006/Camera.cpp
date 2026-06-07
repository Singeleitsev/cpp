//Camera.cpp

//Common headers go first
#include <windows.h>
#include <cstring>

//OpenGL
#include <GL/gl.h>
#include <GL/glu.h>

//Math
#define _USE_MATH_DEFINES
#include <math.h>

#include "Camera.h"

//Const
const double PI_OVER_180 = double(M_PI) / 180.0f;

//Matrix Buffers
double gmBuffer0[16];
double gmBuffer1[16];

Camera::Camera()
{
    Initialize();
};

Camera::~Camera()
{
};

void Camera::Initialize()
{
    vPosition = glm::dvec3(0.0, 4500.0, 9000.0);
    vTarget = glm::dvec3(0.0, 0.0, 0.0);
    vForward = glm::normalize(vTarget - vPosition);
    vWorldUp = glm::dvec3(0.0, 1.0, 0.0);
    qRotation = glm::quatLookAt(vForward, vWorldUp);
    FieldOfView = 90.0;
    AspectRatio = 1.0;
    NearPlane = 0.1;
    FarPlane = 1e6;
};

void Camera::GetDirectionVectors()
{
    vRight = qRotation * glm::dvec3(1.0, 0.0, 0.0);
    vUp = qRotation * glm::dvec3(0.0, 1.0, 0.0);
    vForward = qRotation * glm::dvec3(0.0, 0.0, -1.0);
};

void Camera::GetMatrices()
{
    glm::dmat4 mModelView = getModelViewMatrix();
    glm::dmat4 mProjection = getProjectionMatrix();

    // Copy column‑major data into global double buffers
    memcpy(gmBuffer0, glm::value_ptr(mModelView), sizeof(gmBuffer0));
    memcpy(gmBuffer1, glm::value_ptr(mProjection), sizeof(gmBuffer1));
}

glm::dmat4 Camera::getModelViewMatrix() const
{
    glm::dmat4 rot = glm::toMat4(glm::conjugate(qRotation));
    glm::dmat4 trans = glm::translate(glm::dmat4(1.0), -vPosition);
    return rot * trans;
}

glm::dmat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(FieldOfView), AspectRatio, NearPlane, FarPlane);
}

void Camera::Move(const glm::dvec3& vCameraMove)
{
    vPosition += vCameraMove;
};

void Camera::Pan(const glm::dvec3& vCameraPan)
{
    vPosition += vCameraPan;
};

void Camera::Zoom(double dFOV)
{
    FieldOfView = glm::clamp(FieldOfView - dFOV, 1.0, 120.0);
};

void Camera::Rotate(const glm::dquat& qCameraRot)
{
    qRotation = qCameraRot * qRotation;   // order depends on trackball convention
    qRotation = glm::normalize(qRotation);
};
