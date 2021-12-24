#include "car.h"

/// The constructor sets the static properties of the car, like its size,
/// and then calls Reset() to reset the position, velocity, and any other
/// dynamic variables that change during game play.
Car::Car() : size_(3,2,4), collision_radius_(2.5), accelerationRate_(10), turningRate_(5) {
    Reset();
}

Car::~Car() {
}


float Car::collision_radius() {
    return collision_radius_;
    
}

Vector3 Car::size() {
    return size_;
}

Point3 Car::position() {
    return position_;
}

void Car::set_position(const Point3 &p) {
    position_ = p;
}

Vector3 Car::forward() {
    return forward_;
}

void Car::set_forward(const Vector3 &v) {
    forward_ = v;
}

float Car::speed() {
    return speed_;
}

void Car::set_speed(float s) {
    speed_ = s;
}

Vector3 Car::velocity() {
    return speed_ * forward_;
}


void Car::Reset() {
    position_ = Point3(0, size_[1]/2, 45);
    forward_ = Vector3(0, 0, -1);
    speed_ = 0.0f;
}

float Car::turningRate() {
    return turningRate_;
}
void Car::set_turningRate(float t) {
    turningRate_ = t;
}
float Car::accelerationRate() {
    return accelerationRate_;
}
void Car::set_accelerationRate(float a) {
    accelerationRate_ = a;
}

void Car::Draw(QuickShapes quickShapes, Matrix4 modelMatrix, Matrix4 viewMatrix, Matrix4 projMatrix) {
    Color carcol(0.8f, 0.2f, 0.2f);
    Matrix4 Mcar =
        Matrix4::Translation(position_ - Point3(0,0,0)) *
        Matrix4::LookAt(Point3(0,0,0), Point3(0,0,0) + forward_, Vector3(0,1,0)).Inverse() *
        Matrix4::Scale(size_) *
        Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f));
    quickShapes.DrawCube(modelMatrix * Mcar, viewMatrix, projMatrix, carcol);
}
