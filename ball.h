#ifndef BALL_H_
#define BALL_H_

#include <mingfx.h>
using namespace mingfx;

/// Small class representing the ball.  Feel free to add additional member variables and functions if you wish.
class Ball {
public:
    
    Ball();
    virtual ~Ball();
    
    // The same radius is used to draw the ball and to calculate physics for the ball
    float radius();

    // Current 3D position
    Point3 position();
    void set_position(const Point3 &p);

    // Current 3D velocity
    Vector3 velocity();
    void set_velocity(const Vector3 &v);
    
    // Resets the ball's position and velocity to initial values.
    void Reset();
    
    void Draw(QuickShapes quickShapes, Matrix4 modelMatrix, Matrix4 viewMatrix, Matrix4 projMatrix);
    
private:
    Point3 position_;
    Vector3 velocity_;
    float radius_;
};

#endif
