#include "player.h"
#include "main.h"

Player::Player(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    int num_vert = 10*3;
    int num_sides = num_vert/3;
    GLfloat radius = 1.0f, z= 0.0f;
    GLfloat circleVerticesX[num_vert];
    GLfloat circleVerticesY[num_vert];
    GLfloat circleVerticesZ[num_vert];
    float circle_x=x, circle_y=y+radius;
    circleVerticesX[0] = circle_x;
    circleVerticesY[0] = circle_y;
    circleVerticesZ[0] = z;
    
    for ( int i = 1,k=1; k < num_vert; i++,k++ )
    {
        circleVerticesX[k] = circle_x + ( radius * cos( (i-1) * M_PI /num_sides ) );
        circleVerticesY[k] = circle_y + ( radius * sin( (i-1) * M_PI/ num_sides) );
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
    //semi-circle
    for ( int i = 0; i < num_vert; i++ )
    {
        vertex_buffer_data.push_back(circleVerticesX[i]);
        vertex_buffer_data.push_back(circleVerticesY[i]);
        vertex_buffer_data.push_back( circleVerticesZ[i]);
    }
    //rectangle body
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x-radius,y+radius,z, x+radius,y+radius,z, x+radius,y-radius,z}); // top right triangle
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x+radius,y-radius,z, x-radius,y-radius,z, x-radius,y+radius,z}); // bottom left triangle

    //left leg
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x-radius,y-radius,z, x-(1.0f*radius/6),y-radius,z, x-(1.0f*radius/6),y-2*radius,z}); // top right triangle
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x-(1.0f*radius/6),y-2*radius,z, x-radius,y-2*radius,z, x-radius,y-radius,z}); // bottom left triangle
    
    //right leg
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x+(1.0f*radius/6),y-radius,z, x+radius,y-radius,z, x+radius,y-2*radius+(1.0f*radius/6),z}); // top right triangle
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x+radius,y-2*radius+(1.0f*radius/6),z, x+(1.0f*radius/6),y-2*radius+(1.0f*radius/6),z, x+(1.0f*radius/6),y-radius,z}); // bottom left triangle

    int tot_vert = vertex_buffer_data.size()/3;
    this->object = create3DObject(GL_TRIANGLES, tot_vert, reinterpret_cast<GLfloat *>(vertex_buffer_data.data()), color, GL_FILL);
}

void Player::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube around which we want to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    translate = glm::scale(translate, glm::vec3(0.05, 0.05, 1.0));  
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Player::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Player::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}