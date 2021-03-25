#include "maze.h"
#include "main.h"
#include "input.h"

Maze::Maze(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    std::vector<GLfloat> vertex_buffer_data = {
        // north long line
        -3.0f,3.0f, 0.0f,
        3.0f,3.0f, 0.0f,
        // east long line
        3.0f,-3.0f, 0.0f,
        3.0f, 3.0f, 0.0f,
    };
    std::vector<cell>::iterator temp;
    // bottom left point
    float start_x = START.x; 
    float start_y = START.y; 
    float cell_edge = CELL_SIDE; //cell edge 
    // start_x += cell_edge;
    // start_y -= cell_edge;
    maze = create((int)(6/cell_edge));
    create_graph(maze.size());
    std::vector<std::vector<cell>>::iterator row;
    for(int i = 0; i<maze.size(); i++){
        for(int j=0; j<maze[i].size();j++){
            maze[i][j].pos = point{float(i)*cell_edge+start_x,float(j)*cell_edge+start_y};
            // std::cout<<maze[i][j].s<<maze[i][j].w<<" ";
            if(maze[i][j].s==1){
                vertex_buffer_data.insert( vertex_buffer_data.end() ,{
                    float(i)*cell_edge+start_x, float(j)*cell_edge+start_y, 0.0f,
                    float(i)*cell_edge+start_x+cell_edge, float(j)*cell_edge+start_y, 0.0f
                });
            }
            if(maze[i][j].w==1){
                vertex_buffer_data.insert( vertex_buffer_data.end() ,{
                    float(i)*cell_edge+start_x, float(j)*cell_edge+start_y, 0.0f,
                    float(i)*cell_edge+start_x, float(j)*cell_edge+start_y+cell_edge, 0.0f
                });
            }
        }
        // std::cout<<"\n";
    }
    int vertices = (int)(vertex_buffer_data.size()/3);
    // std::cout<<vertices<<"\n";
    // vertices = 6;

    this->object = create3DObject(GL_LINES, vertices, reinterpret_cast<GLfloat *>(vertex_buffer_data.data()), color, GL_FILL);
}

void Maze::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Maze::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Maze::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

std::vector<std::vector<cell>> Maze::create(int side){
    // int **maze = (int **)malloc(5 * sizeof(int *));
    // for (int i=0; i<5; i++)
    //      maze[i] = (int *)malloc(5 * sizeof(int));
    std::vector<std::vector<cell>> grid;
    std::vector<cell> row;
    for(int i=0; i<side; i++){
        row.clear();
        for(int j=0; j<side; j++){
            row.push_back(cell{point{(float)i,(float)j},1,1});
        }
        grid.push_back(row);
    }
    grid = prims(grid);
    return grid;
}
std::vector<std::vector<cell>> prims(std::vector<std::vector<cell>> grid){
    std::vector<std::vector<int>> visited;
    std::vector<int> row;
    int tot = 0;
    for(int i=0; i<grid.size(); i++){
        row.clear();
        for(int j=0; j<grid[i].size(); j++){
            row.push_back(0);
            tot++;
        }
        visited.push_back(row);
    }
    if(tot<=1){
        return grid;
    }
    visited[0][0] = 1;
    int num_vis = 1;
    int row_tot = grid.size();
    std::vector<point> neighbours;
    int ind = 0;
    neighbours.insert(neighbours.end(),{point{1,0}, point{0,1}});
    visited[0][1]=-1;
    visited[1][0]=-1;
    
    srand (time(NULL));
    while(num_vis<tot){
        if(neighbours.size()==0){
            break;
        }
        ind = rand() % neighbours.size();
        if(visited[neighbours[ind].x][neighbours[ind].y]==1){
            neighbours.erase(neighbours.begin()+ind);
            continue;
        }
        visited[neighbours[ind].x][neighbours[ind].y] = 1;
        num_vis++;
        if(neighbours[ind].x==0){//west of the cell is the boundary
            grid[neighbours[ind].x][neighbours[ind].y].s = 0;
        }
        else if(neighbours[ind].y==0){//south of the cell is the boundary
            grid[neighbours[ind].x][neighbours[ind].y].w = 0;
        }
        else{   // the cell is not in the first column nor in the bottom row
            if(visited[neighbours[ind].x][neighbours[ind].y-1]==1 && visited[neighbours[ind].x-1][neighbours[ind].y]==0)
                grid[neighbours[ind].x][neighbours[ind].y].s = 0;
            else if(visited[neighbours[ind].x][neighbours[ind].y-1]==0 && visited[neighbours[ind].x-1][neighbours[ind].y]==1){
                grid[neighbours[ind].x][neighbours[ind].y].w = 0;
            }
            else{   //both south and west are visited
                int r = rand()%100;
                if(r%2==1){
                    grid[neighbours[ind].x][neighbours[ind].y].s = 0;
                }
                else{
                    grid[neighbours[ind].x][neighbours[ind].y].w = 0;
                }
            }
        }
        int n_x,n_y;
        // left
        n_x = neighbours[ind].x-1;
        n_y=neighbours[ind].y;
        if(n_x>=0 && n_y>=0 && n_x<row_tot && n_y<row_tot){
            if(visited[n_x][n_y]==0){
                neighbours.push_back(point{(float)n_x,(float)n_y});
                visited[n_x][n_y]=-1;
            }
        }
        // right
        n_x = neighbours[ind].x+1;
        if(n_x>=0 && n_y>=0 && n_x<row_tot && n_y<row_tot){
            if(visited[n_x][n_y]==0){
                neighbours.push_back(point{(float)n_x,(float)n_y});
                visited[n_x][n_y]=-1;
            }
        }
        // top
        n_x = neighbours[ind].x;
        n_y=neighbours[ind].y+1;
        if(n_x>=0 && n_y>=0 && n_x<row_tot && n_y<row_tot){
            if(visited[n_x][n_y]==0){
                neighbours.push_back(point{(float)n_x,(float)n_y});
                visited[n_x][n_y]=-1;
            }
        }
        // bottom
        n_y = neighbours[ind].y-1;
        if(n_x>=0 && n_y>=0 && n_x<row_tot && n_y<row_tot){
            if(visited[n_x][n_y]==0){
                neighbours.push_back(point{(float)n_x,(float)n_y});
                visited[n_x][n_y]=-1;
            }
        }
        neighbours.erase(neighbours.begin()+ind);
        
        // std::cout<<num_vis<<" "<<tot<<"\n";
    }
        // std::cout<<"\n";
    // for(int i = 0;i<row_tot;i++){
    //     for(int j= 0; j<row_tot;j++){
    //         std::cout<<visited[i][j]<<" ";
    //     }
    //     std::cout<<"\n";
    // }
        // std::cout<<"\n";
    return grid;

}

/*
bool Maze::wall_collision(bounding_box_t box, point dir){
    float new_x = box.x+dir.x-CELL_SIDE;
    float new_y = box.y+dir.y-CELL_SIDE;
    int i = (new_x-START.x)/CELL_SIDE ;
    int j = (new_y-START.y)/CELL_SIDE ;
    int c_i = (box.x-START.x-CELL_SIDE)/CELL_SIDE ;
    int c_j = (box.y-START.y-CELL_SIDE)/CELL_SIDE ;
    if(i<0 || j<0)
        return true;
    if(i>maze.size()-1 || j>maze.size()-1 )
        return true;
    // std::cout<<"moving in direction: "<<dir.x<<" "<<dir.y<<"\n";
    // std::cout<<"old coordinates: "<<box.x-CELL_SIDE<<" "<<box.y-CELL_SIDE<<"\n";
    // std::cout<<"new coordinates: "<<new_x<<" "<<new_y<<"\n";
    // std::cout<<"box width height: "<<box.width<<" "<<box.height<<"\n";
    // std::cout<<"old cell coordinates: "<<maze[c_i][c_j].pos.x<<" "<<maze[c_i][c_j].pos.y<<"\n";
    // std::cout<<"old cell w s: "<<maze[c_i][c_j].w<<" "<<maze[c_i][c_j].s<<"\n";
    // std::cout<<"cell coordinates: "<<maze[i][j].pos.x<<" "<<maze[i][j].pos.y<<"\n";
    // std::cout<<"cell w s: "<<maze[i][j].w<<" "<<maze[i][j].s<<"\n";
    if(new_x<=float(i)*CELL_SIDE+START.x && maze[i][j].w==1 && dir.x>0)
        return true;
    if(new_y<=float(j)*CELL_SIDE+START.y && maze[i][j].s==1 && dir.y>0)
        return true;
    if(c_i>maze.size()-1 || c_j>maze.size()-1 )
        return true;
    if(maze[c_i][c_j].w==1 && dir.x<0)
        return true;
    if(maze[c_i][c_j].s==1 && dir.y<0)
        return true;
    return false;
}
*/

bool Maze::wall_collision(int i, int j, point dir){
    float new_i = i+dir.x;
    float new_j = j+dir.y;
    if(new_i<0 || new_j<0)
        return true;
    if(new_i>maze.size()-1 || new_j>maze.size()-1 )
        return true;
    if(maze[new_i][new_j].w==1 && dir.x>0)
        return true;
    if( maze[new_i][new_j].s==1 && dir.y>0)
        return true;
    if(i>maze.size()-1 || j>maze.size()-1 ) //not really needed
        return true;
    if(maze[i][j].w==1 && dir.x<0)
        return true;
    if(maze[i][j].s==1 && dir.y<0)
        return true;
    return false;
}

void Maze::create_graph(int s){   
    // std::vector<point> **grap = (std::vector<point>**)malloc(s * sizeof(std::vector<point>*));
    // for (int i=0; i<s; i++)
    //      grap[i] = (std::vector<point>*)malloc(s * sizeof(std::vector<point>));
    // std::vector<point> grap[s][s];
    // std::cout<<"hi\n";
    for(int i=s-1;i>=0;i--){
        for(int j=s-1;j>=0;j--){
            // graph[i][j].clear();
            // std::cout<<"1. i j: "<<i<<" "<<j<<"\n";
            if(maze[i][j].s==0){
                // std::cout<<"south\n";
                graph[i][j].push_back(point{(float)i,(float)j-1});
                graph[i][j-1].push_back(point{(float)i,(float)j});
            }
            if(maze[i][j].w==0){
                // std::cout<<"west\n";
                graph[i][j].push_back(point{(float)i-1,(float)j});
                graph[i-1][j].push_back(point{(float)i,(float)j});
            }
        }
    }
    // for(int i=0; i<s; i++){
    //     for(int j=0;j<s;j++){
    //         std::cout<<"i j: "<<i<<" "<<j<<"\n";
    //         for(auto it=graph[i][j].begin();it!=graph[i][j].end();it++){
    //             std::cout<<it->x<<" "<<it->y<<"\t";
    //         }
    //         std::cout<<"\n";
    //     }
    // }
    // this->graph = grap;
}
