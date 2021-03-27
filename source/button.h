#include "main.h"

#ifndef BUTTON_H
#define BUTTON_H

class Button {
public:
    Button() {}
    Button(int mx, int my, color_t color);
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
    GLfloat radius;
    void press();
    
protected:
    VAO *object;
};

// class Vapouriser : public Button {
// public:
//     Vapouriser():Button(){};
//     Vapouriser(int mx, int my, color_t color):Button(mx,my,color){};
//     void press();
    
// private:
//     VAO *object;
// };

#endif // BUTTON_H
