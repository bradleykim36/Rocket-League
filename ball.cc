#include "ball.h"

/// The constructor sets the radius and calls Reset() to start the ball at
/// the center of the field
Ball::Ball() : radius_(2.6f) {
    Reset();
}
    
Ball::~Ball() {
    
}


float Ball::radius() {
    return radius_;
}
    
Point3 Ball::position() {
    return position_;
}

void Ball::set_position(const Point3 &p) {
    position_ = p;
}

Vector3 Ball::velocity() {
    return velocity_;
}

void Ball::set_velocity(const Vector3 &v) {
    velocity_ = v;
}

void Ball::Reset() {
    position_ = Point3(0, radius_, 0);
    
    float a = GfxMath::PI * rand()/RAND_MAX;
    velocity_ = Vector3(25*cos(a), 10, 25*sin(a));
}

void Ball::Draw(QuickShapes quickShapes, Matrix4 modelMatrix, Matrix4 viewMatrix, Matrix4 projMatrix) {
    Color ballcol(1,1,1);
    Matrix4 Mball =
        Matrix4::Translation(position_- Point3(0,0,0)) *
        Matrix4::Scale(Vector3(radius_, radius_, radius_));
    quickShapes.DrawSphere(modelMatrix * Mball, viewMatrix, projMatrix, ballcol);
    
    // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
    // flattens the sphere into a pancake, which we then draw just a bit
    // above the ground plane.
    Color shadowcol(0.2f, 0.4f, 0.15f);
    Matrix4 Mshadow =
        Matrix4::Translation(Vector3(position_[0], -0.1f, position_[2])) *
        Matrix4::Scale(Vector3(radius_, 0, radius_)) *
        Matrix4::RotationX(90);
    quickShapes.DrawSphere(modelMatrix * Mshadow, viewMatrix, projMatrix, shadowcol);
}
