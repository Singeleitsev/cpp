//Camera.h
#pragma once

#define GLM_ENABLE_EXPERIMENTAL

//D:\bin\dev\ogl\GLM\glm-master\glm
#include <glm.hpp> // Core math types (vec3, mat4)
#include <gtc/quaternion.hpp> // Quaternion support
#include <gtc/matrix_transform.hpp> // Matrix transforms (translate, perspective)
#include <gtx/quaternion.hpp> // Quaternion utilities (needed for `glm::quatLookAt`)
#include <gtc/type_ptr.hpp> // glm::value_ptr function

class Camera {
public:
    Camera();
    ~Camera();

    //Settings
    void Initialize(); //Reset to defaults
    void GetDirectionVectors();
    void GetMatrices();
    glm::dmat4 getModelViewMatrix() const;
    glm::dmat4 getProjectionMatrix() const;

    //Motion
    void Move(const glm::dvec3& vCameraMove); //Arrow Keys, PgUp, PgDn
    void Pan(const glm::dvec3& vCameraPan); //Mouse Wheel Click
    void Zoom(double dFOV); //Mouse Wheel Scroll
    void Rotate(const glm::dquat& qCameraRot); //Right Mouse Button

private:
    //Point of View
    glm::dvec3 vPosition, vTarget, vWorldUp;
    glm::dvec3 vRight, vUp, vForward;
    glm::dquat qRotation;

    //Projection parameters
    GLdouble FieldOfView, AspectRatio, NearPlane, FarPlane;
};
