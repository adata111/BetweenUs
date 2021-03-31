#include "main.h"
#include "timer.h"
#include "maze.h"
#include "player.h"
#include "enemy.h"
#include "button.h"
#include "collectible.h"
#include "input.h"

#define GLT_IMPLEMENTATION
#include "gltext.h"

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
Button release_collectible;
Collectible coins[100];
Collectible bombs[100];

const int NUM_CELLS = 24;
const int NUM_COLLECT = 12;
int SCORE = 0;
int TASKS = 0;
int COUNTDOWN = 60;
int OVER = 0;       // 1 for reactor meltdown, 2 for imposter killing
const float CELL_SIDE = (float)6/NUM_CELLS;
const point START = {-3.0f,-3.0f};              // coordinates of first cell 
const point END = {NUM_CELLS-1, NUM_CELLS-1};   // index of end cell
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
    if(release_collectible.visible)
        release_collectible.draw(VP);
    for(int ind=0; ind<NUM_COLLECT; ind++){
        if(coins[ind].visible)
            coins[ind].draw(VP);
        if(bombs[ind].visible)
            bombs[ind].draw(VP);
    }
    player1.draw(VP);
    if(enemy1.visible)
        enemy1.draw(VP);
}

void drawText(int width, int height){
    
    char score_str[30];
    char time_str[30];
    char tasks_str[30];

    // Creating text
    GLTtext *text = gltCreateText();
    gltSetText(text, "Between Us");
    
    GLTtext *score_text = gltCreateText();
    GLTtext *time_text = gltCreateText();
    GLTtext *tasks_text = gltCreateText();

    // Begin text drawing (this for instance calls glUseProgram)
    gltBeginDraw();

    // Draw any amount of text between begin and end
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2DAligned(text,
    (GLfloat)(width / 2),
    (GLfloat)(40.0f),
    1.5f,
    GLT_CENTER, GLT_TOP);

    sprintf(score_str, "Score: %d", SCORE);
    gltSetText(score_text, score_str);
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(score_text, 100.0f, 80.0f, 1.0f);
    sprintf(time_str, "Time: %d", COUNTDOWN);
	gltSetText(time_text, time_str);
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(time_text, (GLfloat)width - 200, 80.0f, 1.0f);
  
    sprintf(tasks_str, "Tasks: %d/2", TASKS);
	gltSetText(tasks_text, tasks_str);
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2DAligned(tasks_text, (GLfloat)width/2, 80.0f, 1.0f, GLT_CENTER, GLT_TOP);

    // Finish drawing text
    gltEndDraw();

    // Deleting text
    gltDeleteText(text);
    gltDeleteText(score_text);
    gltDeleteText(time_text);
}

void draw_game_over(int width, int height){
    
    char score_str[30]; 
    char time_str[30];
    // Creating text
    GLTtext *text = gltCreateText();
    gltSetText(text, "GAME OVER!");
    GLTtext *result_text = gltCreateText();
    GLTtext *score_text = gltCreateText();
    GLTtext *time_text = gltCreateText();
    GLTtext *mess_text = gltCreateText();

    // Begin text drawing (this for instance calls glUseProgram)
    gltBeginDraw();

    // Draw any amount of text between begin and end
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2DAligned(text,
    (GLfloat)(width / 2),
    (GLfloat)((height / 2)-100),
    1.5f,
    GLT_CENTER, GLT_CENTER);

    if(OVER==3)
        gltSetText(result_text, "YOU WON! :D");
    else
        gltSetText(result_text, "YOU LOST! :(");
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2DAligned(result_text, (GLfloat)(width / 2), (GLfloat)((height / 2)), 2.0f, GLT_CENTER,GLT_CENTER);

    if(OVER==2)
        gltSetText(mess_text, "That imposter was way too fast ;-;");
    else if(OVER==1)
        gltSetText(mess_text, "Try to complete tasks and reach END before reactor meltdown!");
    else if(OVER==3)
        gltSetText(mess_text, "Great Work!");
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2DAligned(mess_text, (GLfloat)(width / 2), (GLfloat)((height / 2)+50), 1.5f, GLT_CENTER,GLT_CENTER);
    
    sprintf(score_str, "Score: %d", SCORE);
    gltSetText(score_text, score_str);
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2DAligned(score_text, (GLfloat)(width / 2), (GLfloat)((height / 2)+150), 1.5f, GLT_CENTER,GLT_CENTER);
    
    sprintf(time_str, "Time left: %d", COUNTDOWN);
	gltSetText(time_text, time_str);
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2DAligned(time_text, (GLfloat)(width/2), (GLfloat)((height / 2)+200), 1.5f, GLT_CENTER, GLT_CENTER);
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
        TASKS +=1;
        SCORE += 50;
    }
    for(int ind=0; ind<NUM_COLLECT; ind++){
        if(coins[ind].visible && coins[ind].maze_x == player1.maze_x && coins[ind].maze_y==player1.maze_y)
            coins[ind].collect();
        if(bombs[ind].visible && bombs[ind].maze_x == player1.maze_x && bombs[ind].maze_y==player1.maze_y)
            bombs[ind].collect();
    }
    if(player1.maze_x==release_collectible.maze_x && player1.maze_y==release_collectible.maze_y && release_collectible.visible){
        release_collectible.visible=0;
        for(int ind=0; ind<NUM_CELLS; ind++){
            coins[ind].visible=1;
            bombs[ind].visible=1;
        }
        TASKS+=1;
        SCORE += 50;
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    
    srand (time(NULL));
    maze1       = Maze(0, 0, COLOR_BLACK);
    player1     = Player(0,0, COLOR_BLUE);
    enemy1     = Enemy(1+(NUM_CELLS/2), NUM_CELLS/2, COLOR_RED);
    vapouriser   = Button(NUM_CELLS/6 + rand()%(5*NUM_CELLS/6), 5*NUM_CELLS/6 + rand()%(NUM_CELLS/6), COLOR_RED_TRUE);
    release_collectible   = Button(NUM_CELLS/6 + rand()%(5*NUM_CELLS/6), NUM_CELLS/6 + rand()%(5*NUM_CELLS/6), COLOR_GREEN);
    for(int ind=0;ind<NUM_COLLECT;ind++){
        coins[ind] = Collectible( NUM_CELLS/6 + rand()%(5*NUM_CELLS/6), rand()%(NUM_CELLS), COLOR_YELLOW, 10);
        bombs[ind] = Collectible( rand()%(NUM_CELLS-1), rand()%(NUM_CELLS), COLOR_BLACK, -10);
        // bombs[ind] = Collectible( (ind%2)*(NUM_CELLS/3) + rand()%(NUM_CELLS/2), ((ind+1)%2)*(NUM_CELLS/3) + rand()%(NUM_CELLS/2), COLOR_BLACK, -10);
    } 

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
    int viewport_width, viewport_height;
    bool check = false;

    window = initGLFW(width, height);

    initGL (window, width, height);
    // Initialize glText
    gltInit();
    
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            glfwGetFramebufferSize(window, &viewport_width, & viewport_height);
            if(!OVER){    
                draw();
                drawText(viewport_width, viewport_height);
                // Swap Frame Buffer in double buffering
                glfwSwapBuffers(window);

                // tick_elements();
                tick_input(window);
                check_obj_collision();
                if(enemy1.visible)
                    enemy1.check_player_collision(point{(float)player1.maze_x, (float)player1.maze_y});

            }
            else{
                if(!check){
                    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    draw_game_over(viewport_width, viewport_height);
                    // Swap Frame Buffer in double buffering
                    glfwSwapBuffers(window);
                }
                // tick_elements();
                tick_input(window);
            }
        }
        if (t1.processTick() && !OVER){
            // 1 fps
            std::cout<<"Score: "<<SCORE<<"\n";
            COUNTDOWN--;
            if(COUNTDOWN<=0)
                OVER= 1;    // reactor meltdown
            if(enemy1.visible)
                enemy1.move_dijkstra(maze1.graph, point{(float)player1.maze_x, (float)player1.maze_y});
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    // Destroy glText
    gltTerminate();

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
