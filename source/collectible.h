#include "main.h"

#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

class Collectible {
public:
    Collectible() {}
    Collectible(int mx, int my, color_t color, int points);
    glm::vec3 position;
    glm::vec3 start_pos;
    float scale_size;
    float rotation;
    int maze_x;
    int maze_y;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    float speed;
    bool visible;
    int points;
    GLfloat radius;
    void collect();
    
private:
    VAO *object;
};

#endif // COLLECTIBLE_H
