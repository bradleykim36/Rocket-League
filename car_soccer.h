#ifndef CAR_SOCCER_H_
#define CAR_SOCCER_H_

#include <mingfx.h>
using namespace mingfx;

#include "ball.h"
#include "car.h"


// The main class for the Car Soccer application
class CarSoccer : public GraphicsApp {
public:
    CarSoccer();
    virtual ~CarSoccer();

    /// Called whenever the mouse moves
    void OnMouseMove(const Point2& pos, const Vector2& delta);

    /// This is called when special keys like SPACEBAR are pressed
    void OnSpecialKeyDown(int key, int scancode, int modifiers);

    /// This is called once each frame.  dt is "delta time", the time elapsed
    /// since the last call.
    void UpdateSimulation(double dt);
    
    /// This is called when it is time to initialize graphics objects, like
    /// texture files.
    void InitOpenGL();
    
    /// This is called once each frame, and you should draw the scene inside
    /// this function.
    void DrawUsingOpenGL();
    
    /// This is a little utility function that is helpful.  It treats the
    /// arrow keys like a joystick and returns the direction you are pressing
    /// as a 2D vector, taking into account the fact that you might be holding
    /// down more than one key at a time.
    Vector2 joystick_direction();
    
    // Feel free to add more functions here as needed.

    
private:

    // Simulation objects/parameters:
    
    // We suggest you start with the Car and Ball objects provided, adding new
    // member variables to those classes if you need to.  You'll probably want
    // to store some other data for the simulation here too, like some value
    // for gravity.
    Car car_;
    Ball ball_;
    
    
    // Support for drawing some simple shapes:
    QuickShapes quickShapes_;
    
    // Images to use as textures:
    Texture2D fieldTex_;
    Texture2D crowdTex_;

    // Control the computer graphics camera (we'll learn about this in a few weeks):
    Matrix4 modelMatrix_;
    Matrix4 viewMatrix_;
    Matrix4 projMatrix_;
    
    // A list of paths to search for data files (images):
    std::vector<std::string> searchPath_;
    
    // Set this to true if you want to use the mouse to control the car rather than the keyboard
    bool use_mouse_;

    // Mouse position in Normalized Device Coordinates, 
    // meaning -1 to +1 in both X and Y.  (0,0) is the
    // center of the screen
    Point2 mouse_pos_;
};


#endif
