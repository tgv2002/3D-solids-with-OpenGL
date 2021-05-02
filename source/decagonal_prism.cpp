#include "decagonal_prism.h"
#include "main.h"

GLfloat DecagonalPrism::v_dec_prism_top_vertices[90];
GLfloat DecagonalPrism::v_dec_prism_bottom_vertices[90];
GLfloat DecagonalPrism::v_dec_prism_middle_vertices[360];
GLfloat DecagonalPrism::c_dec_prism_top_vertices[90];
GLfloat DecagonalPrism::c_dec_prism_bottom_vertices[90];
GLfloat DecagonalPrism::c_dec_prism_middle_vertices[360];

DecagonalPrism::DecagonalPrism(float x, float y) {
    this->position = glm::vec3(x, y, 0.0f);
    this->rotation = 40 + 90;
    this->is_spinning = 0;
    speed = 1;

    // decagonal prism top
    getRegularPolygon(0.0f, 0.265f, 0.0f, 0.6f, 10, 1.0f, 1.0f, 0.0f);
    for(int j=0;j<180;j++){
        this->dec_prism_top_vertices[j] = this->triangleArrayP[j];
    }
    for(int j=0;j<60;j++){
        this->dec_prism_top_polygon[j] = this->polygonArray[j];
    }

    // decagonal prism bottom
    getRegularPolygon(0.0f, -0.265f, 0.0f, 0.6, 10, 1.0f, 0.5f, 0.0f);
    for(int j=0;j<180;j++)
        this->dec_prism_bottom_vertices[j] = this->triangleArrayP[j];
    for(int j=0;j<60;j++){
        this->dec_prism_bottom_polygon[j] = this->polygonArray[j];
    }

    // decagonal prism middle part -- 10 squares
    getQuadsBetweenPolygons(10);
    for(int j=0;j<720;j++)
        this->dec_prism_middle_vertices[j] = this->squareArray[j];

    splitVerticesAndColors();
    this->object1 = create3DObject(GL_TRIANGLES, 30, this->v_dec_prism_top_vertices, this->c_dec_prism_top_vertices, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 120, this->v_dec_prism_middle_vertices, this->c_dec_prism_middle_vertices, GL_FILL);
    this->object3 = create3DObject(GL_TRIANGLES, 30, this->v_dec_prism_bottom_vertices, this->c_dec_prism_bottom_vertices, GL_FILL);
}

void DecagonalPrism::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float)(40.0 * M_PI / 180.0f), glm::vec3(1, 0, 0));
    if(this->is_spinning)
        rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
}

void DecagonalPrism::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void DecagonalPrism::set_spin_status(int should_spin){
    this->is_spinning = should_spin;
}

void DecagonalPrism::tick() {
    this->rotation += speed;
}

// Get a list of colors such that each face has a unique color
void DecagonalPrism::getColorsList(int groupSize){
    float availableColors[] = {
        1.0f, 0.0f, 0.0f,       // red
        0.0f, 1.0f, 0.0f,       // green
        0.0f, 0.0f, 1.0f,       // blue
        0.0f, 0.5f, 0.0f,       // dark green
        1.0f, 0.0f, 1.0f,       // purple
        0.0f, 1.0f, 1.0f,       // cyan
        0.85f, 0.85f, 0.10f,    // gold
        0.44f, 0.57f, 0.86f,    // turquoise
        0.22f, 0.69f, 0.87f,    // summer Sky blue
        1.00f, 0.11f, 0.68f,    // pink
        1.0f, 1.0f, 0.0f,       // yellow
        1.0f, 0.5f, 0.0f        // orange

    };

    int count = 0;
    for(int i=0;i<36;i+=3){
        for(int j=0;j<groupSize;j++){
            this->colorsList[count++] = availableColors[i];
            this->colorsList[count++] = availableColors[i+1];
            this->colorsList[count++] = availableColors[i+2];
        }
    }
}

// Create an n-sided polygon with n triangles (join centre of polygon to n vertices to get n triangles)
void DecagonalPrism::makePolygonWithTriangles(float centre_x, float centre_y,
                            float centre_z, int polygonVertices, float color_x, 
                            float color_y, float color_z){
    int vertexCounter = 0;

    for(int i=0;i<3*6*polygonVertices;i+=18){
        this->triangleArrayP[i] = centre_x;
        this->triangleArrayP[i+1] = centre_y;
        this->triangleArrayP[i+2] = centre_z;
        this->triangleArrayP[i+3] = color_x;
        this->triangleArrayP[i+4] = color_y;
        this->triangleArrayP[i+5] = color_z;

        this->triangleArrayP[i+6] = this->polygonArray[vertexCounter++];
        this->triangleArrayP[i+7] = this->polygonArray[vertexCounter++];
        this->triangleArrayP[i+8] = this->polygonArray[vertexCounter++];
        this->triangleArrayP[i+9] = color_x;
        this->triangleArrayP[i+10] = color_y;
        this->triangleArrayP[i+11] = color_z;
        vertexCounter += 3;

        this->triangleArrayP[i+12] = this->polygonArray[vertexCounter % (6*polygonVertices)];
        this->triangleArrayP[i+13] = this->polygonArray[(vertexCounter+1) % (6*polygonVertices)];
        this->triangleArrayP[i+14] = this->polygonArray[(vertexCounter+2) % (6*polygonVertices)];
        this->triangleArrayP[i+15] = color_x;
        this->triangleArrayP[i+16] = color_y;
        this->triangleArrayP[i+17] = color_z;
    } 
}

// Obtain regular polygon with given number of vertices and given centre, color and distances
void DecagonalPrism::getRegularPolygon(float centre_x, float centre_y, float centre_z, 
                        float r, int polygonVertices, float color_x, 
                        float color_y, float color_z){
    float theta = 0.0;
    float theta_diff = 360.0 / polygonVertices;

    for(int i=0;i<6*polygonVertices;i+=6){
        this->polygonArray[i] = centre_x + (r * cos(theta * 3.14159/180));
        this->polygonArray[i+1] = centre_y;
        this->polygonArray[i+2] = centre_z + (r * sin(theta * 3.14159/180));
        this->polygonArray[i+3] = color_x;
        this->polygonArray[i+4] = color_y;
        this->polygonArray[i+5] = color_z;
        theta += theta_diff;
    }
    makePolygonWithTriangles(centre_x, centre_y, centre_z,
                            polygonVertices, color_x, color_y, color_z);
}

// Obtain 10 square between top and bottom decagons, based on vertices of the 2 decagons
void DecagonalPrism::getQuadsBetweenPolygons(int polygonVertices){
    getColorsList(4);
    int colorCounter = 0;
    int vertexCounter1 = 0;
    int vertexCounter2 = 0;
    float centre_x, centre_y, centre_z;

    for(int i=0;i<12*6*polygonVertices;i+=72){
        this->polygonArray[0] = this->dec_prism_top_polygon[vertexCounter1++];
        this->polygonArray[1] = this->dec_prism_top_polygon[vertexCounter1++];
        this->polygonArray[2] = this->dec_prism_top_polygon[vertexCounter1++];
        this->polygonArray[3] = this->colorsList[colorCounter++];
        this->polygonArray[4] = this->colorsList[colorCounter++];
        this->polygonArray[5] = this->colorsList[colorCounter++];
        vertexCounter1 += 3;

        this->polygonArray[6] = this->dec_prism_top_polygon[vertexCounter1 % (6*polygonVertices)];
        this->polygonArray[7] = this->dec_prism_top_polygon[(vertexCounter1+1) % (6*polygonVertices)];
        this->polygonArray[8] = this->dec_prism_top_polygon[(vertexCounter1+2) % (6*polygonVertices)];
        this->polygonArray[9] = this->colorsList[colorCounter++];
        this->polygonArray[10] = this->colorsList[colorCounter++];
        this->polygonArray[11] = this->colorsList[colorCounter++];

        this->polygonArray[12] = this->dec_prism_bottom_polygon[(vertexCounter2+6) % (6*polygonVertices)];
        this->polygonArray[13] = this->dec_prism_bottom_polygon[(vertexCounter2+7) % (6*polygonVertices)];
        this->polygonArray[14] = this->dec_prism_bottom_polygon[(vertexCounter2+8) % (6*polygonVertices)];
        this->polygonArray[15] = this->colorsList[colorCounter++];
        this->polygonArray[16] = this->colorsList[colorCounter++];
        this->polygonArray[17] = this->colorsList[colorCounter++];

        this->polygonArray[18] = this->dec_prism_bottom_polygon[vertexCounter2++];
        this->polygonArray[19] = this->dec_prism_bottom_polygon[vertexCounter2++];
        this->polygonArray[20] = this->dec_prism_bottom_polygon[vertexCounter2++];
        this->polygonArray[21] = this->colorsList[colorCounter++];
        this->polygonArray[22] = this->colorsList[colorCounter++];
        this->polygonArray[23] = this->colorsList[colorCounter++];
        vertexCounter2 += 3;

        centre_x = (this->polygonArray[0] + this->polygonArray[18]) / 2.0;
        centre_y = (this->polygonArray[1] + this->polygonArray[19]) / 2.0;
        centre_z = (this->polygonArray[2] + this->polygonArray[20]) / 2.0;

        
        makePolygonWithTriangles(centre_x, centre_y, centre_z, 4, 
                                this->polygonArray[3], this->polygonArray[4], this->polygonArray[5]);
        for(int j=0;j<72;j++)
            this->squareArray[i+j] = this->triangleArrayP[j];
    }
}

// Split the single list containing vertices and colors into separate lists for convenience
void DecagonalPrism::splitVerticesAndColors(){
    int v_ctr = 0;
    int c_ctr = 0;

    for(int i=0;i<180;i++){
        if(i % 6 < 3)
            this->v_dec_prism_top_vertices[v_ctr++] = this->dec_prism_top_vertices[i];
        else
            this->c_dec_prism_top_vertices[c_ctr++] = this->dec_prism_top_vertices[i];
    }

    v_ctr = 0; 
    c_ctr = 0;

    for(int i=0;i<180;i++){
        if(i % 6 < 3)
            this->v_dec_prism_bottom_vertices[v_ctr++] = this->dec_prism_bottom_vertices[i];
        else
            this->c_dec_prism_bottom_vertices[c_ctr++] = this->dec_prism_bottom_vertices[i];
    }

    v_ctr = 0; 
    c_ctr = 0;

    for(int i=0;i<720;i++){
        if(i % 6 < 3)
            this->v_dec_prism_middle_vertices[v_ctr++] = this->dec_prism_middle_vertices[i];
        else
            this->c_dec_prism_middle_vertices[c_ctr++] = this->dec_prism_middle_vertices[i];
    }
}