#include "main.h"

#ifndef MAZE_H
#define MAZE_H

struct point {
    float x;
    float y;
};
struct cell {
    point pos;
    int w;
    int s;
};


class Maze {
public:
    Maze() {}
    Maze(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    std::vector<std::vector<cell>> create(int s);
private:
    VAO *object;
};
std::vector<std::vector<cell>> prims(std::vector<std::vector<cell>> grid);

#endif // MAZE_H
