#include "main.h"

#ifndef MAZE_H
#define MAZE_H

struct cell {
    point pos;
    int w;
    int s;
};


class Maze {
public:
    Maze() {}
    Maze(float x, float y, color_t color);
    std::vector<std::vector<cell>> maze;
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    std::vector<std::vector<cell>> create(int s);
    void create_graph(int s);
    std::vector<point> graph[100][100];
    bool wall_collision(bounding_box_t box, point dir);
private:
    VAO *object;
};
std::vector<std::vector<cell>> prims(std::vector<std::vector<cell>> grid);

#endif // MAZE_H
