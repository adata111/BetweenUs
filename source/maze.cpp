#include "maze.h"
#include "main.h"

Maze::Maze(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    std::vector<GLfloat> vertex_buffer_data = {
        // -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        // -1.0f,-1.0f, 1.0f,
        // -1.0f, 1.0f, 1.0f, // triangle 1 : end
        // 1.0f, 1.0f,-1.0f, // triangle 2 : begin
        // -1.0f,-1.0f,-1.0f,
        // -1.0f, 1.0f,-1.0f, // triangle 2 : end
        // 1.0f,-1.0f, 1.0f,
        // -1.0f,-1.0f,-1.0f,
        // 1.0f,-1.0f,-1.0f,
        // 1.0f, 1.0f,-1.0f,
        // 1.0f,-1.0f,-1.0f,
        // -1.0f,-1.0f,-1.0f,
        // -1.0f,-1.0f,-1.0f,
        // -1.0f, 1.0f, 1.0f,
        // -1.0f, 1.0f,-1.0f,
        // 1.0f,-1.0f, 1.0f,
        // -1.0f,-1.0f, 1.0f,
        // -1.0f,-1.0f,-1.0f,
        // -1.0f, 1.0f, 1.0f,
        // -1.0f,-1.0f, 1.0f,
        // 1.0f,-1.0f, 1.0f,
        // 1.0f, 1.0f, 1.0f,
        // 1.0f,-1.0f,-1.0f,
        // 1.0f, 1.0f,-1.0f,
        // 1.0f,-1.0f,-1.0f,
        // 1.0f, 1.0f, 1.0f,
        // 1.0f,-1.0f, 1.0f,
        // 1.0f, 1.0f, 1.0f,
        // 1.0f, 1.0f,-1.0f,
        // -1.0f, 1.0f,-1.0f,
        // 1.0f, 1.0f, 1.0f,
        // -1.0f, 1.0f,-1.0f,
        // -1.0f, 1.0f, 1.0f,
        // 3.0f, 3.0f, 0.0f,
        // -3.0f, 3.0f, 0.0f,
        // -3.0f, 3.0f, 0.0f,
        // -3.0f,-3.0f, 0.0f,
        // north long line
        -3.0f,3.0f, 0.0f,
        3.0f,3.0f, 0.0f,
        // east long line
        3.0f,-3.0f, 0.0f,
        3.0f, 3.0f, 0.0f,
    };
    std::vector<cell>::iterator temp;
    // bottom left point
    float start_x = -3.0f; 
    float start_y = -3.0f; 
    float cell_edge = 0.25f; //cell edge 
    // start_x += cell_edge;
    // start_y -= cell_edge;
    std::vector<std::vector<cell>> maze = create((int)(6/cell_edge));
    std::vector<std::vector<cell>>::iterator row;
    for(int i = 0; i<maze.size(); i++){
        for(int j=0; j<maze[i].size();j++){
            std::cout<<maze[i][j].s<<maze[i][j].w<<" ";
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
        std::cout<<"\n";
    }
    int vertices = (int)(vertex_buffer_data.size()/3);
    std::cout<<vertices<<"\n";
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
            if(i==1 && j==1)
            row.push_back(cell{point{(float)i,(float)j},1,0});
            else
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
        
        std::cout<<num_vis<<" "<<tot<<"\n";
    }
        std::cout<<"\n";
    for(int i = 0;i<row_tot;i++){
        for(int j= 0; j<row_tot;j++){
            std::cout<<visited[i][j]<<" ";
        }
        std::cout<<"\n";
    }
        std::cout<<"\n";
    return grid;

}

