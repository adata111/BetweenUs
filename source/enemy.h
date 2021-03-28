#include "main.h"

#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
public:
    Enemy() {}
    Enemy(int x, int y, color_t color);
    glm::vec3 position;
    glm::vec3 start_pos;
    bounding_box_t bounds;
    float scale_size;
    float rotation;
    int maze_x;
    int maze_y;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float speed;
    bool visible;
    GLfloat radius;
    
    void right();
    void left();
    void up();
    void down();
    
    void move_dijkstra(std::vector<point> graph[100][100], point player);
    // point dijkstra(int x, int y, std::vector<point> graph[100][100], std::vector<std::vector<int>> vis, int dest_x, int dest_y, point dir_moved);
private:
    VAO *object;
};

#endif // ENEMY_H
