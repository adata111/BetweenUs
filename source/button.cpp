#include "button.h"
#include "main.h"

Button::Button(int mx, int my, color_t color) {
    this->maze_x = mx;
    this->maze_y = my;
    this->position = glm::vec3((START.x+(CELL_SIDE* float(mx+0.5))), (START.y+(CELL_SIDE* float(my+0.5))), 0);
    this->start_pos = this->position;
    // std::cout<<"button coords "<<start_pos.x<<" "<<start_pos.y<<"\n";
    float x = position.x;
    float y = position.y;
    this->visible = 1;
    this->rotation = 0;
    radius = 1.0f;
    scale_size= (float)CELL_SIDE/3;
    // scale_size = 1.0f;
    // radius *= scale_size;
    speed = CELL_SIDE;
    int num_vert = 20*3;
    int num_sides = num_vert/3;
    GLfloat z= 0.0f;
    GLfloat circleVerticesX[num_vert];
    GLfloat circleVerticesY[num_vert];
    GLfloat circleVerticesZ[num_vert];
    float circle_x=x, circle_y=y;
    circleVerticesX[0] = circle_x;
    circleVerticesY[0] = circle_y;
    circleVerticesZ[0] = z;
    
    for ( int i = 1,k=1; k < num_vert; i++,k++ )
    {
        circleVerticesX[k] = circle_x + ( radius * cos( (i-1) * 2 * M_PI /num_sides ) );
        circleVerticesY[k] = circle_y + ( radius * sin( (i-1) * 2 * M_PI/ num_sides) );
        circleVerticesZ[k] = z;
        if(k%3==2){
            ++k;
            if(k>=num_vert)
                break;
            circleVerticesX[k] = circleVerticesX[0];
            circleVerticesY[k] = circleVerticesY[0];
            circleVerticesZ[k] = z;
            
            ++k;
            if(k>=num_vert)
                break;
            circleVerticesX[k] = circleVerticesX[k-2];
            circleVerticesY[k] = circleVerticesY[k-2];
            circleVerticesZ[k] = z;
        }
    }
    
    std::vector<GLfloat> vertex_buffer_data;
    //circle
    for ( int i = 0; i < num_vert; i++ )
    {
        vertex_buffer_data.push_back(circleVerticesX[i]);
        vertex_buffer_data.push_back(circleVerticesY[i]);
        vertex_buffer_data.push_back( circleVerticesZ[i]);
    }

    int tot_vert = vertex_buffer_data.size()/3;
    this->object = create3DObject(GL_TRIANGLES, tot_vert, reinterpret_cast<GLfloat *>(vertex_buffer_data.data()), color, GL_FILL);
}

void Button::draw(glm::mat4 VP) {
     Matrices.model = glm::mat4(1.0f);
    // std::cout<<maze_x<<" "<<maze_y<<"   "<<position.x<<" "<<position.y<<"\n";
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    glm::mat4 neg_translate = glm::translate (-this->start_pos);
    glm::mat4 scale = glm::scale(glm::vec3(scale_size, scale_size, 1.0));
    // rotate = rotate * glm::translate(-this->position);
    // scale = scale * glm::translate(-this->position);
    Matrices.model *= (translate * rotate * scale * neg_translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}


void Button::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

