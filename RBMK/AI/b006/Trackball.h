//Trackball.h
#pragma once


#include <glm.hpp>
#include <gtc/quaternion.hpp>

class Trackball {
public:
    Trackball();
    void setViewport(int width, int height);
    void startDrag(double x, double y);   // mouse down
    void drag(double x, double y);        // mouse move while down
    void endDrag();                       // optional: reset
    glm::dquat getRotation() const;       // rotation delta since drag started
    bool isDragging() const;

private:
    glm::dvec2 projectToSphere(double x, double y) const;
    int viewportWidth, viewportHeight;
    glm::dvec3 lastPoint;
    glm::dquat deltaRotation;
    bool dragging;
};