#include "player.h"
#include "main.h"

Player::Player(int mx, int my, color_t color) {
    this->maze_x = mx;
    this->maze_y = my;
    this->position = glm::vec3((START.x+(CELL_SIDE* float(mx+0.5))), (START.y+(CELL_SIDE* float(my+0.5))), 0);
    this->start_pos = this->position;
    // std::cout<<"//////////////////////////////////////\n";
    std::cout<<position.x<<" "<<position.y<<"\n";
    // std::cout<<pos.x<<" "<<pos.y<<"\n";
    // this->position = glm::vec3(0.0f,0.0f,0.0f);
    float x = position.x;
    float y = position.y;
    this->rotation = 0;
    radius = 1.0f;
    scale_size= (float)CELL_SIDE/5;
    // radius *= scale_size;
    // this->bounds = {position.x,position.y,2*radius*scale_size,radius*4*scale_size};
    speed = CELL_SIDE;
    int num_vert = 10*3;
    int num_sides = num_vert/3;
    GLfloat z= 0.0f;
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
    std::vector<GLfloat> colour_buffer_data;
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

    //rectangle backpack
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x-(9.0f*radius/6),y+radius,z, x-radius,y+radius,z, x-radius,y-(5.0f*radius/6),z}); // top right triangle
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x-radius,y-(5.0f*radius/6),z, x-(9.0f*radius/6),y-(5.0f*radius/6),z, x-(9.0f*radius/6),y+radius,z}); // bottom left triangle

    //left leg
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x-radius,y-radius,z, x-(1.0f*radius/6),y-radius,z, x-(1.0f*radius/6),y-2*radius,z}); // top right triangle
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x-(1.0f*radius/6),y-2*radius,z, x-radius,y-2*radius,z, x-radius,y-radius,z}); // bottom left triangle
    
    //right leg
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x+(1.0f*radius/6),y-radius,z, x+radius,y-radius,z, x+radius,y-2*radius+(1.0f*radius/6),z}); // top right triangle
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x+radius,y-2*radius+(1.0f*radius/6),z, x+(1.0f*radius/6),y-2*radius+(1.0f*radius/6),z, x+(1.0f*radius/6),y-radius,z}); // bottom left triangle

    //rectangle helmet
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x-(3.0f*radius/6),y+(8.0f*radius/6),z, x+(7.0f*radius/6),y+(8.0f*radius/6),z, x+(7.0f*radius/6),y+(2.0f*radius/6),z}); // top right triangle
    vertex_buffer_data.insert(vertex_buffer_data.end(), {x+(7.0f*radius/6),y+(2.0f*radius/6),z, x-(3.0f*radius/6),y+(2.0f*radius/6),z, x-(3.0f*radius/6),y+(8.0f*radius/6),z}); // bottom left triangle

    int tot_vert = vertex_buffer_data.size()/3;
    for(int i=0;i<(tot_vert-3*2);i++){
        colour_buffer_data.insert(colour_buffer_data.end(),{(float)color.r/256,(float)color.g/256,(float)color.b/256});
    }
    // helmet colour
    for(int i=0;i<3*2;i++){
        colour_buffer_data.insert(colour_buffer_data.end(),{(float)(COLOR_GREY.r)/256,(float)(COLOR_GREY.g)/256,(float)(COLOR_GREY.b)/256});
    }
    // std::cout<<"vertex: "<<vertex_buffer_data.size()<<"\tcolour: "<<colour_buffer_data.size()<<"\n";
    this->object = create3DObject(GL_TRIANGLES, tot_vert, reinterpret_cast<const GLfloat *>(vertex_buffer_data.data()), reinterpret_cast<const GLfloat *>(colour_buffer_data.data()), GL_FILL);
}

void Player::draw(glm::mat4 VP) {
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
    Matrices.model *= (translate * rotate * scale * neg_translate );
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}


void Player::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Player::tick() {
    this->rotation += 1;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

void Player::right(){
    this->position.x += speed;
}
void Player::left(){
    this->position.x -= speed;
}
void Player::up(){
    this->position.y += speed;
}
void Player::down(){
    this->position.y -= speed;
}

void Player::move(point dir){
    int mx=this->maze_x;int my=this->maze_y;
    if(mx==END.x && my==END.y && TASKS==2){
        OVER=3;
    }
    this->position.x += speed*dir.x;
    this->position.y += speed*dir.y;
    this->maze_x += dir.x;
    this->maze_y += dir.y;
    // this->position = glm::vec3((START.x+(CELL_SIDE* float(mx+0.5))), (START.y+(CELL_SIDE* float(my+0.5))), 0);
    this->bounds.x = this->position.x;
    this->bounds.y = this->position.y;
}
