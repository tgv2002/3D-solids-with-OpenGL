#include "main.h"

#ifndef UNDECAGONAL_PYRAMID_H
#define UNDECAGONAL_PYRAMID_H


class UndecagonalPyramid {
public:
    UndecagonalPyramid() {}
    UndecagonalPyramid(float x, float y);
    glm::vec3 position;
    float rotation;
    int is_spinning;

    // UNDECAGONAL PYRAMID ARRAYS
    float undec_pyr_top_vertices[198];
    float undec_pyr_bottom_vertices[198];

    // UNDECAGONAL PYRAMID ARRAYS VERTEX SEPARATED
    static GLfloat v_undec_pyr_top_vertices[99];
    static GLfloat v_undec_pyr_bottom_vertices[99];

    // UNDECAGONAL PYRAMID ARRAYS COLORS SEPARATED
    static GLfloat c_undec_pyr_top_vertices[99];
    static GLfloat c_undec_pyr_bottom_vertices[99];

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

#endif // UNDECAGONAL_PYRAMID_H