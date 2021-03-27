#include "main.h"
#include "timer.h"
#include "maze.h"
#include "player.h"
#include "enemy.h"
#include "button.h"
#include "input.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Maze maze1;
Player player1;
Enemy enemy1;
Button vapouriser;

const int NUM_CELLS = 24;
const float CELL_SIDE = (float)6/NUM_CELLS;
const point START = {-3.0f,-3.0f};
const point END = {3.0f-CELL_SIDE, 3.0f-CELL_SIDE};
std::vector<std::vector<int>> vis;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);
Timer t1(1.0);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 0, 0, 5 );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    maze1.draw(VP);
    if(vapouriser.visible)
        vapouriser.draw(VP);
    player1.draw(VP);
    if(enemy1.visible)
        enemy1.draw(VP);
}

void tick_input(GLFWwindow *window) {

    if(key_down){
        if(maze1.wall_collision(player1.maze_x, player1.maze_y, point{0,-1})==false)
            player1.move(point{0,-1});
        key_down = false;
    }
    else if(key_up){
        if(maze1.wall_collision(player1.maze_x, player1.maze_y, point{0,1})==false)
            player1.move(point{0,1});
        key_up = false;
    }
    else if(key_right){
        if(maze1.wall_collision(player1.maze_x, player1.maze_y, point{1,0})==false)
            player1.move(point{1,0});
        key_right = false;
    }
    else if(key_left){
        if(maze1.wall_collision(player1.maze_x, player1.maze_y, point{-1,0})==false)
            player1.move(point{-1,0});
        key_left = false;
    }
    
}

void tick_elements() {
    player1.tick();
    // camera_rotation_angle += 1;
}

void check_obj_collision(){
    if(player1.maze_x==vapouriser.maze_x && player1.maze_y==vapouriser.maze_y && vapouriser.visible){
        vapouriser.visible=0;
        enemy1.visible=0;
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    maze1       = Maze(0, 0, COLOR_BLACK);
    player1     = Player(0,0, COLOR_BLUE);
    enemy1     = Enemy(13, 12, maze1.maze[13][12].pos, COLOR_RED);
    vapouriser   = Button(NUM_CELLS/2, (NUM_CELLS-1), COLOR_GREEN);
    // player1     = Player(-1.42, -1.42, COLOR_RED);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 900;
    int height = 900;

    window = initGLFW(width, height);

    initGL (window, width, height);
    
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            // tick_elements();
            tick_input(window);
            check_obj_collision();
        }
        if (t1.processTick()){
            if(enemy1.visible)
                enemy1.move_dijkstra(maze1.graph, point{(float)player1.maze_x, (float)player1.maze_y});
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
