#include "main.h"

#ifndef HEXAGONAL_DIPYRAMID_H
#define HEXAGONAL_DIPYRAMID_H


class HexagonalDiPyramid {
public:
    HexagonalDiPyramid() {}
    HexagonalDiPyramid(float x, float y);
    glm::vec3 position;
    float rotation;
    int is_spinning;

    // HEXAGONAL DIPYRAMID ARRAYS
    float hex_dipyr_top_vertices[108];
    float hex_dipyr_bottom_vertices[108];

    // HEXAGONAL DIPYRAMID ARRAYS VERTEX SEPARATED
    static GLfloat v_hex_dipyr_top_vertices[54];
    static GLfloat v_hex_dipyr_bottom_vertices[54];

    // HEXAGONAL DIPYRAMID ARRAYS COLORS SEPARATED
    static GLfloat c_hex_dipyr_top_vertices[54];
    static GLfloat c_hex_dipyr_bottom_vertices[54];

    // FEW TEMP ARRAYS
    float colorsList[150];
    float triangleArray[200];
    float triangleArrayP[200];
    float squareArray[720];
    float polygonArray[66];

    // FEW HELPER FUNCTIONS
    void getColorsList(int groupSize);
    void makePolygonWithTriangles(float centre_x, float centre_y,
                                float centre_z, int polygonVertices, float color_x, 
                                float color_y, float color_z);
    void getRegularPolygon(float centre_x, float centre_y, float centre_z, 
                        float r, int polygonVertices, float color_x, 
                        float color_y, float color_z);
    void splitVerticesAndColors();
    void getTrianglesOnPolygon(float ext_x, float ext_y, float ext_z, int polygonVertices);

    void draw(glm::mat4 VP);
    void set_spin_status(int should_spin);
    void set_position(float x, float y, float z);
    void tick();
    double speed;
private:
    VAO *object1;
    VAO *object2;
};

#endif // HEXAGONAL_DIPYRAMID_H