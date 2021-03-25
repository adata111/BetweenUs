#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player() {}
    Player(int x, int y, color_t color);
    glm::vec3 position;
    bounding_box_t bounds;
    int maze_x;
    int maze_y;
    float scale_size;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float speed;
    GLfloat radius;
    
    void right();
    void left();
    void up();
    void down();
    
    void move(point dir);
private:
    VAO *object;
};

#endif // PLAYER_H
