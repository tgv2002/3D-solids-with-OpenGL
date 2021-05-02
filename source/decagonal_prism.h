#include "main.h"

#ifndef DECAGONAL_PRISM_H
#define DECAGONAL_PRISM_H


class DecagonalPrism {
public:
    DecagonalPrism() {}
    DecagonalPrism(float x, float y);
    glm::vec3 position;
    float rotation;
    int is_spinning;

    // DECAGONAL PRISM ARRAYS
    float dec_prism_top_vertices[180];
    float dec_prism_bottom_vertices[180];
    float dec_prism_top_polygon[60];
    float dec_prism_bottom_polygon[60];
    float dec_prism_middle_vertices[720];

    // DECAGONAL PRISM ARRAYS VERTEX SEPARATED
    static GLfloat v_dec_prism_top_vertices[90];
    static GLfloat v_dec_prism_bottom_vertices[90];
    static GLfloat v_dec_prism_middle_vertices[360];

    // DECAGONAL PRISM ARRAYS COLORS SEPARATED
    static GLfloat c_dec_prism_top_vertices[90];
    static GLfloat c_dec_prism_bottom_vertices[90];
    static GLfloat c_dec_prism_middle_vertices[360];

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
    void getQuadsBetweenPolygons(int polygonVertices);
    void splitVerticesAndColors();

    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void set_spin_status(int should_spin);
    void tick();
    double speed;
private:
    VAO *object1;
    VAO *object2;
    VAO *object3;
};

#endif // DECAGONAL_PRISM_H