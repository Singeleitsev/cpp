//Trackball.cpp

//Common headers go first
#include <cmath>

#include "Trackball.h"

static const double SPHERE_RADIUS = 1.0;

Trackball::Trackball() : viewportWidth(1), viewportHeight(1), dragging(false) {
    deltaRotation = glm::dquat(1.0, 0.0, 0.0, 0.0);
}

void Trackball::setViewport(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
}

glm::dvec2 Trackball::projectToSphere(double x, double y) const {
    // Normalize coords to [-1, 1] range (Y flipped)
    double nx = (x - viewportWidth / 2.0) / (viewportWidth / 2.0);
    double ny = (viewportHeight / 2.0 - y) / (viewportHeight / 2.0);
    double r2 = nx * nx + ny * ny;
    if (r2 <= 0.5) {
        // Inside sphere
        double z = sqrt(1.0 - r2);
        return glm::dvec2(nx, ny);
    }
    else {
        // Project onto sphere edge
        return glm::dvec2(nx, ny) / sqrt(r2);
    }
}

void Trackball::startDrag(double x, double y) {
    dragging = true;
    glm::dvec2 p2d = projectToSphere(x, y);
    lastPoint = glm::dvec3(p2d.x, p2d.y, sqrt(1.0 - p2d.x * p2d.x - p2d.y * p2d.y));
    deltaRotation = glm::dquat(1.0, 0.0, 0.0, 0.0); // reset delta
}

void Trackball::drag(double x, double y) {
    if (!dragging) return;
    glm::dvec2 p2d = projectToSphere(x, y);
    glm::dvec3 curPoint(p2d.x, p2d.y, sqrt(1.0 - p2d.x * p2d.x - p2d.y * p2d.y));
    // Compute rotation axis = cross(lastPoint, curPoint)
    glm::dvec3 axis = glm::cross(lastPoint, curPoint);
    double dot = glm::dot(lastPoint, curPoint);
    double angle = std::acos(std::min(1.0, std::max(-1.0, dot)));
    if (angle > 1e-6 && glm::length(axis) > 1e-6) {
        axis = glm::normalize(axis);
        glm::dquat rot = glm::angleAxis(angle, axis);
        deltaRotation = rot * deltaRotation;   // accumulate
        lastPoint = curPoint;
    }
}

void Trackball::endDrag() {
    dragging = false;
}

glm::dquat Trackball::getRotation() const {
    return deltaRotation;
}

bool Trackball::isDragging() const {
    return dragging;
}