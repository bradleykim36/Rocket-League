#include "car_soccer.h"
#include "config.h"


// Remember in C++, the .h file list all the functions and member variables that are part of the class!
// Look there first to understand what is part of the CarSoccer class, then look below to see how each
// function is implemented.


CarSoccer::CarSoccer() : GraphicsApp(1024, 768, "Car Soccer") {
    // If you are having trouble driving the car with the keybaord, you can set this to true to use
    // the mouse instead.  The mouse controls are based on the postion of the mouse cursor on the window.
    // There is a "dead zone" in the middle of the window, and if you move the mouse up/down or left/right
    // outside of that zone, it is like pushing the up/down and/or left/right keys on the keyboard
    use_mouse_ = false;

    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


void CarSoccer::OnMouseMove(const Point2& pos, const Vector2& delta)
{
    mouse_pos_ = PixelsToNormalizedDeviceCoords(pos);
}

void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
        ball_.Reset();
    }
}

/// This is a little utility function that is helpful.  It treats the arrow keys like a joystick or D-pad on a game controller
/// and returns the direction you are pressing as a 2D vector, taking into account the fact that you might be holding
/// down more than one key at a time.
Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;

    if (use_mouse_) {
        // threshold defines the size of the "dead zone" in the middle of the screen
        // if the mouse's x,y position falls outside of this, then it is like pushing
        // the corresponding key on the keyboard
        const float threshold = 0.2f;
        dir[0] = 0;
        if (mouse_pos_[0] < -threshold) {
            dir[0] = -1;
        }
        else if (mouse_pos_[0] > threshold) {
            dir[0] = 1;
        }
        dir[1] = 0;
        if (mouse_pos_[1] < -threshold) {
            dir[1] = -1;
        }
        else if (mouse_pos_[1] > threshold) {
            dir[1] = 1;
        }
    }
    else {
        // the default user interface is to use the arrow keys on the keyboard.
        // like a D-pad on a game controller, you can hold more than one key down at a time if you want.
        if (IsKeyDown(GLFW_KEY_LEFT))
            dir[0]--;
        if (IsKeyDown(GLFW_KEY_RIGHT))
            dir[0]++;
        if (IsKeyDown(GLFW_KEY_UP))
            dir[1]++;
        if (IsKeyDown(GLFW_KEY_DOWN))
            dir[1]--;
    }

    return dir;
}

// dt is for "Delta Time", the elapsed time in seconds since the last frame
void CarSoccer::UpdateSimulation(double dt) {
    Vector2 dpad_dir = joystick_direction();
    std::cout << "D-Pad Direction: " << dpad_dir << std::endl;

    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball based on the elapsed time and checking for collisions.  Filling
    // in this routine is the main part of the assignment.
    //dt = fmin(timeStep, 0.1);


    //increase the ball's acceleration in the y-direction to be greater than regular gravity for better gameplay
    ball_.set_velocity(ball_.velocity() + Vector3(0, -15 * dt, 0));

   
    //Checks to see if the ball is inside the goal
    if ((ball_.position()[0] <= (10 - ball_.radius())) && ball_.position()[0] >= (ball_.radius() - 10)) {
        if (ball_.position()[1] <= (10 - ball_.radius()) && ball_.position()[2] <= (ball_.radius() - 50) || ball_.position()[2] >= (50 - ball_.radius())) {
            ball_.Reset();
            car_.Reset();
        }
    }
    // Checking the ball's contact with it's x-coordinate, and slowing the velocity on contact + resetting its position to be in the field
    if (ball_.position()[0] >= (40 - ball_.radius())) {
        ball_.set_velocity(Vector3(0.8 * -abs(ball_.velocity()[0]), 0.8 * ball_.velocity()[1], 0.8 * ball_.velocity()[2]));
        ball_.set_position(Point3(40 - ball_.radius(), ball_.position()[1], ball_.position()[2]));
    }
    if (ball_.position()[0] < (ball_.radius() - 40)) {
        ball_.set_velocity(Vector3(0.8 * abs(ball_.velocity()[0]), 0.8 * ball_.velocity()[1], 0.8 * ball_.velocity()[2]));
        ball_.set_position(Point3(ball_.radius() - 40, ball_.position()[1], ball_.position()[2]));
    }
    // Checking the ball's contact with it's y-coordinate, and slowing the velocity on contact + resetting its position to be in the field
    if (ball_.position()[1] < (ball_.radius())) {
        ball_.set_velocity(Vector3(0.8*ball_.velocity()[0],0.8*abs(ball_.velocity()[1]),0.8*ball_.velocity()[2]));
        ball_.set_position(Point3(ball_.position()[0], ball_.radius(),ball_.position()[2]));
    }
    if (ball_.position()[1] >= (35 - ball_.radius())) {
        ball_.set_velocity(Vector3(0.8*ball_.velocity()[0],0.8*-abs(ball_.velocity()[1]),0.8*ball_.velocity()[2]));
        ball_.set_position(Point3(ball_.position()[0],35 - ball_.radius(),ball_.position()[2]));
    }
    // Checking the ball's contact with it's z-coordinate, and slowing the velocity on contact + resetting its position to be in the field
    if (ball_.position()[2] >= (50 - ball_.radius())) {
        ball_.set_velocity(Vector3(0.8*ball_.velocity()[0], 0.8*ball_.velocity()[1], 0.8*-abs(ball_.velocity()[2])));
        ball_.set_position(Point3(ball_.position()[0], ball_.position()[1], 50 - ball_.radius()));
    }
    if (ball_.position()[2] < (-50 + ball_.radius())) {
        ball_.set_velocity(Vector3(0.8 * ball_.velocity()[0], 0.8 * ball_.velocity()[1], 0.8 * abs(ball_.velocity()[2])));
        ball_.set_position(Point3(ball_.position()[0], ball_.position()[1], ball_.radius() - 50));
    }
    
    // Checks car collision with the rightmost wall and stops the car's velocity accordingly
    if (car_.position()[0] > (40 - car_.collision_radius())) {
        car_.set_position(Point3(40 - car_.collision_radius(),car_.position()[1],car_.position()[2]));
        car_.set_speed(0);
    }
    // Checks car collision with the leftmost wall and stops the car's velocity accordingly
    if (car_.position()[0] < (-40 + car_.collision_radius())) {
        car_.set_position(Point3(-40 + car_.collision_radius(), car_.position()[1], car_.position()[2]));
        car_.set_speed(0);
    }
    // Checks car collision with the far goal wall and stops the car's velocity accordingly
    if (car_.position()[2] < (car_.collision_radius() - 50)) {
        car_.set_position(Point3(car_.position()[0], car_.position()[1], car_.collision_radius() - 50));
        car_.set_speed(0);
    }
    // Checks car collision with the close goal wall and stops the car's velocity accordingly
    if (car_.position()[2] > (50 - car_.collision_radius())) {
        car_.set_position(Point3(car_.position()[0], car_.position()[1], 50 - car_.collision_radius()));
        car_.set_speed(0);
    }
    //Vector2 direction will handle the arrow inputs
    Vector2 direction = joystick_direction();

    // I wasn't able to get this part of the project to work, I wasn't sure how to implement the turningRate and accelerationRate functions to make my car move accordingly
    if (direction[1] == GLFW_KEY_UP) {
        if (car_.speed() < 100) {
            car_.set_speed(car_.speed() + car_.accelerationRate());
        }
    }
    if (direction[1] == GLFW_KEY_DOWN) {
        if (car_.speed() > 0) {
            car_.set_speed(car_.speed() - car_.accelerationRate());
        }
    }
    //I have commented out the work I have so far so that the program runs without error
    if (direction[0] == GLFW_KEY_RIGHT) {
        //clockwise: car_.set_forward((cos(car_.turningRate() * dt) * car_.forward()[0] + sin(car_.turningRate() * dt) * car_.forward()[2], 0, -sin(car_.turningRate() * dt) * car_.forward()[0] + 
        //cos(car_.turningRate() * dt) * car_.forward()[2]);
    }
    
    if (direction[0] == GLFW_KEY_LEFT) {
        //counter_clockwise: car_.set_forward((cos(car_.turningRate() * dt) * car_.forward()[0] - sin(car_.turningRate()) * car_.forward()[2], 0, sin(car_.turningRate() * dt) * car_.forward()[0] + 
        //cos(car_.turningRate() * dt) * car_.forward()[2]);
    }
    car_.set_position(car_.position() + car_.velocity() * dt);

    // Collission between the ball and car, and how their velocities change depending on the collision based on normalized vector
    Vector3 collision = ball_.position() - car_.position();
    Vector3 collisionNormalized = Vector3::Normalize(collision);
    // Determines the ball and car collision
    if (collision.Length() < (ball_.radius() + car_.collision_radius())) {
        ball_.set_position(car_.position() + (ball_.radius() + car_.collision_radius()) * collisionNormalized);
        ball_.set_velocity(car_.velocity() + ((ball_.velocity() - car_.velocity()) - 1.8 * ((ball_.velocity() - car_.velocity()).Dot(collisionNormalized) * collisionNormalized)));
    }
    ball_.set_position(ball_.position() + (ball_.velocity() * dt));
}
void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));
 
    // Set a background color for the screen (don't worry if you get a depricated warning on this line in OSX)
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
}
void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);

    // Draw the car and the ball
    car_.Draw(quickShapes_, modelMatrix_, viewMatrix_, projMatrix_);
    ball_.Draw(quickShapes_, modelMatrix_, viewMatrix_, projMatrix_);
    
    // Draw the field with the field texture on it.
    Color col(16.0f/255.0f, 46.0f/255.0f, 9.0f/255.0f);
    Matrix4 M = Matrix4::Translation(Vector3(0.0f, -0.201f, 0.0f)) * Matrix4::Scale(Vector3(50.0f, 1.0f, 60.0f));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0.0f, -0.2f, 0.0f)) * Matrix4::Scale(Vector3(40.0f, 1.0f, 50.0f));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);
    
    // You should add drawing the goals and the boundary of the playing area
    // using quickShapes_.DrawLines()
    //Setting the car's color and drawing the car
    Color carColor(1, 0, 0);
    Matrix4 Matrix4Car = Matrix4::Translation(car_.position() - Point3(0, 0, 0)) * Matrix4::RotationY(car_.turningRate()) * Matrix4::Scale(car_.size()) * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));
    quickShapes_.DrawCube(modelMatrix_ * Matrix4Car, viewMatrix_, projMatrix_, carColor);
    // Setting the ball's color and drawing the ball
    Color ballColor(1, 1, 1);
    Matrix4 Matrix4Ball = Matrix4::Translation(ball_.position() - Point3(0, 0, 0)) * Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
    quickShapes_.DrawSphere(modelMatrix_ * Matrix4Ball, viewMatrix_, projMatrix_,ballColor);

    Color lineColor = Color(1, 1, 1);

    // Drawing the right field lines
    Matrix4 Matrix4RightLines;
    std::vector<Point3> rightLines;
    rightLines.push_back(Point3(40, 0, -50));
    rightLines.push_back(Point3(40, 0, 50));
    rightLines.push_back(Point3(40, 35, -50));
    rightLines.push_back(Point3(40, 35, 50));
    quickShapes_.DrawLines(modelMatrix_ * Matrix4RightLines, viewMatrix_, projMatrix_, lineColor, rightLines, QuickShapes::LinesType::LINES, 0.1);

    // Drawing the left field lines
    Matrix4 Matrix4LeftLines;
    std::vector<Point3> leftLines;
    leftLines.push_back(Point3(-40, 0, -50));
    leftLines.push_back(Point3(-40, 0, 50));
    leftLines.push_back(Point3(-40, 35, -50));
    leftLines.push_back(Point3(-40, 35, 50));
    quickShapes_.DrawLines(modelMatrix_ * Matrix4LeftLines, viewMatrix_, projMatrix_, lineColor, leftLines, QuickShapes::LinesType::LINES, 0.1);

    // Drawing the upper field lines
    Matrix4 Matrix4UpperLines;
    std::vector<Point3> upperLines;
    upperLines.push_back(Point3(-40, 35, -50));
    upperLines.push_back(Point3(40, 35, -50));
    upperLines.push_back(Point3(40, 0, -50));
    upperLines.push_back(Point3(-40, 0, -50));
    quickShapes_.DrawLines(modelMatrix_ * Matrix4UpperLines, viewMatrix_, projMatrix_, lineColor, upperLines, QuickShapes::LinesType::LINE_LOOP, 0.1);

    // Drawing the lower field lines
    Matrix4 Matrix4LowerLines;
    std::vector<Point3> lowerLines;
    lowerLines.push_back(Point3(-40, 35, 50));
    lowerLines.push_back(Point3(40, 35, 50));
    lowerLines.push_back(Point3(40, 0, 50));
    lowerLines.push_back(Point3(-40, 0, 50));
    quickShapes_.DrawLines(modelMatrix_ * Matrix4LowerLines, viewMatrix_, projMatrix_, lineColor, lowerLines, QuickShapes::LinesType::LINE_LOOP, 0.1);

    // Drawing the far goal lines
    Matrix4 Matrix4FarGoal;
    Color bCol(0, 1, 1);
    std::vector<Point3> farGoal;
    farGoal.push_back(Point3(-10, 0, -50));
    farGoal.push_back(Point3(10, 0, -50));
    farGoal.push_back(Point3(10, 10, -50));
    farGoal.push_back(Point3(-10, 10, -50));
    quickShapes_.DrawLines(modelMatrix_ * Matrix4FarGoal, viewMatrix_, projMatrix_, bCol, farGoal, QuickShapes::LinesType::LINE_LOOP, 0.2);

    // Draw the close goal lines
    Matrix4 Matrix4CloseGoal;
    Color closeGoalColor(1, 0.5, 0);
    std::vector<Point3> closeGoal;
    closeGoal.push_back(Point3(-10, 0, 50));
    closeGoal.push_back(Point3(10, 0, 50));
    closeGoal.push_back(Point3(10, 10, 50));
    closeGoal.push_back(Point3(-10, 10, 50));
    quickShapes_.DrawLines(modelMatrix_ * Matrix4CloseGoal, viewMatrix_, projMatrix_, closeGoalColor, closeGoal, QuickShapes::LinesType::LINE_LOOP, 0.2);

    // Drawing the far goal net
    std::vector<Point3> farNet;
    for (int x = -10; x <= 10; x++) {
        farNet.push_back(Point3(x, 0, -50));
        farNet.push_back(Point3(x, 10, -50));
    }
    for (int y = 2; y <= 10; y++) {
        farNet.push_back(Point3(-10, y, -50));
        farNet.push_back(Point3(10, y, -50));
    }
    quickShapes_.DrawLines(modelMatrix_ * Matrix4FarGoal, viewMatrix_, projMatrix_, bCol, farNet, QuickShapes::LinesType::LINES, 0.2);

    // Draw the close goal net
    std::vector<Point3> closeNet;
    for (int x = -10; x <= 10; x++) {
        closeNet.push_back(Point3(x, 0, 50));
        closeNet.push_back(Point3(x, 10, 50));
    }
    for (int y = 2; y <= 10; y++) {
        closeNet.push_back(Point3(-10, y, 50));
        closeNet.push_back(Point3(10, y, 50));
    }
    quickShapes_.DrawLines(modelMatrix_ * Matrix4CloseGoal, viewMatrix_, projMatrix_, closeGoalColor, closeNet, QuickShapes::LinesType::LINES, 0.2);
}
