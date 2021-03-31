#include "imposter.h"
#include "main.h"

Imposter::Imposter(int mx, int my, color_t color) {
    this->maze_x = mx;
    this->maze_y = my;
    this->position = glm::vec3((START.x+(CELL_SIDE* float(mx+0.5))), (START.y+(CELL_SIDE* float(my+0.5))), 0);
    this->start_pos = this->position;
    // std::cout<<"//////////////////////////////////////\n";
    // std::cout<<position.x<<" "<<position.y<<"\n";
    // std::cout<<pos.x<<" "<<pos.y<<"\n";
    // this->position = glm::vec3(0.0f,0.0f,0.0f);
    float x = position.x;
    float y = position.y;
    this->visible = 1;
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
    // Make the imposter face left i.e. flip along y axis
    for(int i=0;i<3*tot_vert;i+=3){
        vertex_buffer_data[i] = -vertex_buffer_data[i];
    }
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

void Imposter::draw(glm::mat4 VP) {
     Matrices.model = glm::mat4(1.0f);
    // this->position = glm::vec3(START.x + (float)CELL_SIDE*this->maze_x, START.y + (float)CELL_SIDE*this->maze_y, 0.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    
    glm::mat4 neg_translate = glm::translate (-this->start_pos);
    glm::mat4 scale = glm::scale(glm::vec3(scale_size, scale_size, 1.0));

    Matrices.model *= (translate * rotate * scale * neg_translate );
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Imposter::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Imposter::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

void Imposter::right(){
    this->position.x += speed;
}
void Imposter::left(){
    this->position.x -= speed;
}
void Imposter::up(){
    this->position.y += speed;
}
void Imposter::down(){
    this->position.y -= speed;
}

// A utility function to find the
// vertex with minimum distance
// value, from the set of vertices
// not yet included in shortest
// path tree
point minDistance(std::vector<std::vector<int>> dist,
				std::vector<std::vector<int>> visSet)
{
	// Initialize min value
	int min = INT_MAX;
    point min_index = {-1,-1};

	for (int v = 0; v < NUM_CELLS; v++){
        for(int i=0; i<NUM_CELLS; i++){
            if (visSet[v][i] == 0 && dist[v][i] <= min){
                min = dist[v][i];
                min_index.x = v;
                min_index.y = i;
            }
        }
    }
	return min_index;
}

bool Imposter::check_player_collision(point player){
    if(this->visible==false)
        return false;
    if(player.x==maze_x && player.y==maze_y){
        std::cout<<"collide\n";
        OVER = 2;
        return true;
    }
    return false;
}

void Imposter::move_dijkstra(std::vector<point> graph[100][100], point player){
    if(check_player_collision(player)){
        return;
    }
    std::vector<std::vector<int>> visited;
    std::vector<std::vector<int>> dist;
    std::vector<std::vector<point>> par;
    std::vector<int> row;
    std::vector<int> row_dist;
    std::vector<point> row_par;
    int tot = 0;
    int inf=1e+9;
    for(int i=0; i<NUM_CELLS; i++){
        // std::cout<<i<<"\n";
        row.clear();
        row_dist.clear();
        row_par.clear();
        for(int j=0; j<NUM_CELLS; j++){
            row.push_back(0);
            row_par.push_back(point{-1,-1});
            // if(i==0 && j==0)
            //     row_dist.emplace_back(0);
            // else
                row_dist.emplace_back(inf);
            tot++;
        }
        visited.push_back(row);
        dist.emplace_back(row_dist);
        par.push_back(row_par);
        // std::cout<<i<<"\n";
    }
    if(tot<=1){
        return;
    }
    int i = player.x, j=player.y;
    // std::cout<<i<<" "<<j<<"\n";
    dist[i][j]=0;
    point temp;
    while(1){
        // std::cout<<i<<"\n";
        for(auto it=graph[i][j].begin();it!=graph[i][j].end();it++){
            if(visited[it->x][it->y])
                continue;
            if(dist[it->x][it->y] > dist[i][j]+1){
                dist[it->x][it->y] = dist[i][j]+1;
                par[it->x][it->y] = point{(float)i, (float)j};
            }
        }
        visited[i][j]=1;
        if(i==maze_x && j==maze_y){
            // for(int a=0;a<NUM_CELLS;a++){
            //     for(int b = 0; b<NUM_CELLS; b++)
            //         std::cout<<dist[a][b]<<" ";
            //     std::cout<<"\n";
            // }
            // std::cout<<"dist: "<<dist[i][j]<<"\n";
            // std::cout<<"par: "<<par[i][j].x<<" "<<par[i][j].y<<"\n";
            break;
        }
        temp=minDistance(dist, visited);
        i=temp.x;
        j=temp.y;
        if(i==-1 && j==-1)
            break;
        
    }
    if(i!=-1 && j!=-1){
        this->maze_x = par[i][j].x;
        this->maze_y = par[i][j].y;
        this->position = glm::vec3((START.x+(CELL_SIDE* float(maze_x+0.5))), (START.y+(CELL_SIDE* float(maze_y+0.5))), 0);
    }
    
}
