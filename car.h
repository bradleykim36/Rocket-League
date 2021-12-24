#ifndef CAR_H_
#define CAR_H_

#include <mingfx.h>
using namespace mingfx;

/// Small class representing the car.  Feel free to add additional member variables and functions if you wish.
class Car {
public:
    Car();
    virtual ~Car();
        
    // Size is the size of the box drawn to represent the car in the X, Y, and Z dimensions
    Vector3 size();

    // Radius of the bounding sphere used to calculate approximate physics
    float collision_radius();

    // 3D position of the car
    Point3 position();
    void set_position(const Point3 &p);

    // The unit vector direction the car is pointing.  Since the car drives around on the
    // Y=0 plane, this vector will always have a y coordinate = 0
    Vector3 forward();
    void set_forward(const Vector3 &v);

    // The speed of the car can be positive (moving forward) or negative (moving in reverse)
    float speed();
    void set_speed(float s);
    
    // Current 3D velocity, computed as speed_ * forward_
    Vector3 velocity();
    
    // Resets the position, speed, and forward direction to the car's starting point
    void Reset();
    
    // Draws a simple box shape for the car at the proper position and rotated to face forward
    void Draw(QuickShapes quickShapes, Matrix4 modelMatrix, Matrix4 viewMatrix, Matrix4 projMatrix);
    
    float accelerationRate();
    void set_accelerationRate(float a);

    float turningRate();
    void set_turningRate(float t);

private:
    Vector3 size_;
    float collision_radius_;
    Point3 position_;
    Vector3 forward_;
    float speed_;
    Vector3 velocity_;
    float accelerationRate_;
    float turningRate_;
};

#endif
