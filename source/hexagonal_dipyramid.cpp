#include "hexagonal_dipyramid.h"
#include "main.h"

GLfloat HexagonalDiPyramid::v_hex_dipyr_top_vertices[54];
GLfloat HexagonalDiPyramid::v_hex_dipyr_bottom_vertices[54];
GLfloat HexagonalDiPyramid::c_hex_dipyr_top_vertices[54];
GLfloat HexagonalDiPyramid::c_hex_dipyr_bottom_vertices[54];

HexagonalDiPyramid::HexagonalDiPyramid(float x, float y) {
    this->position = glm::vec3(x, y, 0.0f);
    this->rotation = 0 + 90;
    this->is_spinning = 0;
    speed = 1;

    getRegularPolygon(0.0f, 0.0f, 0.0f, 0.35f, 6, 0.5f, 1.0f, 0.5f);
    
    // hexagonal dipyramid top faces
    getTrianglesOnPolygon(0.0f, 0.50f, 0.0f, 6);
    for(int j=0;j<108;j++)
        this->hex_dipyr_top_vertices[j] = this->triangleArray[j];

    // hexagonal dipyramid bottom faces
    getTrianglesOnPolygon(0.0f, -0.50f, 0.0f, 6);
    for(int j=0;j<108;j++)
        this->hex_dipyr_bottom_vertices[j] = this->triangleArray[j];

    splitVerticesAndColors();
    this->object1 = create3DObject(GL_TRIANGLES, 18, this->v_hex_dipyr_top_vertices, this->c_hex_dipyr_top_vertices, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 18, this->v_hex_dipyr_bottom_vertices, this->c_hex_dipyr_bottom_vertices, GL_FILL);
}

void HexagonalDiPyramid::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float)(this->is_spinning * this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
}

void HexagonalDiPyramid::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void HexagonalDiPyramid::set_spin_status(int should_spin){
    this->is_spinning = should_spin;
}

void HexagonalDiPyramid::tick() {
    this->rotation += speed;
}

// Get a list of colors such that each face has a unique color
void HexagonalDiPyramid::getColorsList(int groupSize){
    float availableColors[] = {
        1.0f, 0.0f, 0.0f,       // red
        0.0f, 1.0f, 0.0f,       // green
        0.0f, 0.0f, 1.0f,       // blue
        0.0f, 0.5f, 0.0f,       // dark green
        1.0f, 0.0f, 1.0f,       // purple
        0.0f, 1.0f, 1.0f,       // cyan
        0.85f, 0.85f, 0.10f,    // gold
        1.0f, 0.5f, 0.0f,       // orange
        0.22f, 0.69f, 0.87f,    // summer Sky blue
        1.00f, 0.11f, 0.68f,    // pink
        1.0f, 1.0f, 0.0f,       // yellow
        0.44f, 0.57f, 0.86f    // turquoise
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
void HexagonalDiPyramid::makePolygonWithTriangles(float centre_x, float centre_y,
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
void HexagonalDiPyramid::getRegularPolygon(float centre_x, float centre_y, float centre_z, 
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

// Obtain 6 triangles on desired side of polygon based on external vertex and polygon vertices
void HexagonalDiPyramid::getTrianglesOnPolygon(float ext_x, float ext_y, float ext_z, int polygonVertices){
    getColorsList(3);
    int colorCounter = (ext_y < -0.1 && polygonVertices == 6) ? 54 : 0;
    int vertexCounter = 0;

    for(int i=0;i<3*6*polygonVertices;i+=18){
        this->triangleArray[i] = ext_x;
        this->triangleArray[i+1] = ext_y;
        this->triangleArray[i+2] = ext_z;
        this->triangleArray[i+3] = this->colorsList[colorCounter++];
        this->triangleArray[i+4] = this->colorsList[colorCounter++];
        this->triangleArray[i+5] = this->colorsList[colorCounter++];

        this->triangleArray[i+6] = this->polygonArray[vertexCounter++];
        this->triangleArray[i+7] = this->polygonArray[vertexCounter++];
        this->triangleArray[i+8] = this->polygonArray[vertexCounter++];
        this->triangleArray[i+9] = this->colorsList[colorCounter++];
        this->triangleArray[i+10] = this->colorsList[colorCounter++];
        this->triangleArray[i+11] = this->colorsList[colorCounter++];
        vertexCounter += 3;

        this->triangleArray[i+12] = this->polygonArray[vertexCounter % (6*polygonVertices)];
        this->triangleArray[i+13] = this->polygonArray[(vertexCounter+1) % (6*polygonVertices)];
        this->triangleArray[i+14] = this->polygonArray[(vertexCounter+2) % (6*polygonVertices)];
        this->triangleArray[i+15] = this->colorsList[colorCounter++];
        this->triangleArray[i+16] = this->colorsList[colorCounter++];
        this->triangleArray[i+17] = this->colorsList[colorCounter++];
    }
}

// Split the single list containing vertices and colors into separate lists for convenience
void HexagonalDiPyramid::splitVerticesAndColors(){
    int v_ctr = 0;
    int c_ctr = 0;

    for(int i=0;i<108;i++){
        if(i % 6 < 3)
            this->v_hex_dipyr_top_vertices[v_ctr++] = this->hex_dipyr_top_vertices[i];
        else
            this->c_hex_dipyr_top_vertices[c_ctr++] = this->hex_dipyr_top_vertices[i];
    }

    v_ctr = 0; 
    c_ctr = 0;

    for(int i=0;i<108;i++){
        if(i % 6 < 3)
            this->v_hex_dipyr_bottom_vertices[v_ctr++] = this->hex_dipyr_bottom_vertices[i];
        else
            this->c_hex_dipyr_bottom_vertices[c_ctr++] = this->hex_dipyr_bottom_vertices[i];
    }
}