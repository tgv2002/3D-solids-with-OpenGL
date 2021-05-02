#include "main.h"
#include "timer.h"
#include "decagonal_prism.h"
#include "hexagonal_dipyramid.h"
#include "undecagonal_pyramid.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

DecagonalPrism dec_prism;
HexagonalDiPyramid hex_dipyr;
UndecagonalPyramid undec_pyr;
int current_fig_displaying = 0;
int spin_camera_about_object = 0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

// unit vectors for translation
glm::vec3 i_cap = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 j_cap = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 k_cap = glm::vec3(0.0f, 0.0f, 1.0f);

// camera
glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 camera_front_vector = glm::vec3(0.0f, 0.0f, -1.0f);

glm::vec3 camera_eye = camera_position;
glm::vec3 camera_target = camera_position + camera_front_vector;
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f,  0.0f);

// Fixed teleportation positions
glm::vec3 camera_fixed_position1_eye = glm::vec3(-2.12f, 0.0f, 2.12f);
glm::vec3 camera_fixed_position1_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_fixed_position1_up = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 camera_fixed_position2_eye = glm::vec3(2.12f, 0.0f, 2.12f);
glm::vec3 camera_fixed_position2_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_fixed_position2_up = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 camera_fixed_position3_eye = glm::vec3(-3.0f, 0.0f, 0.0f);
glm::vec3 camera_fixed_position3_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_fixed_position3_up = glm::vec3(0.0f, 1.0f, 0.0f);

const float camera_speed = 0.11f;
const float object_speed = 0.10f;

// Translates object for any of 3 shapes
template <class T> 
void translate_object(T &display_object, int axis, float diff){

    if(axis == 0){
         display_object.set_position(display_object.position.x + diff, display_object.position.y, display_object.position.z);
    }
    else if(axis == 1){
        display_object.set_position(display_object.position.x, display_object.position.y + diff, display_object.position.z);
    }
    else{
        display_object.set_position(display_object.position.x, display_object.position.y, display_object.position.z + diff);
    }
}

// Spins object for any of 3 shapes
template <class T> 
void spin_object(T &display_object){
    display_object.set_spin_status(1);
}

// Stops Spinning object for any of 3 shapes
template <class T> 
void unspin_object(T &display_object){
    display_object.set_spin_status(0);
}

// Translates camera along axes
void camera_shift(glm::vec3 shift_vector){
    camera_position += (shift_vector);
    camera_eye = camera_position;
    camera_target = camera_position + camera_front_vector;  
}

/* Render the scene with openGL */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    glUseProgram (programID);

    if(spin_camera_about_object == 1){
        glm::vec3 target_object = dec_prism.position;
        if(current_fig_displaying == 1){
            target_object = hex_dipyr.position;
        }
        else if(current_fig_displaying == 2){
            target_object = undec_pyr.position;
        }
        camera_position += glm::normalize(glm::cross(camera_front_vector, camera_up)) * camera_speed;
        camera_eye = camera_position;
        camera_target = target_object;
        camera_front_vector = camera_target - camera_eye;
    }

    // Camera view
    Matrices.view = glm::lookAt(camera_eye, camera_target, camera_up);
    Matrices.projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    if(current_fig_displaying == 0){
        dec_prism.draw(VP);
    } else if(current_fig_displaying == 1){
        hex_dipyr.draw(VP);
    } else {
        undec_pyr.draw(VP);
    }
}

void tick_input(GLFWwindow *window) {

    // Switching between objects
    int get_dec_prism = glfwGetKey(window, GLFW_KEY_1);
    int get_hex_dipyr = glfwGetKey(window, GLFW_KEY_2);
    int get_undec_pyr = glfwGetKey(window, GLFW_KEY_3);

    // Moving camera
    int move_camera_left = glfwGetKey(window, GLFW_KEY_A);
    int move_camera_right = glfwGetKey(window, GLFW_KEY_D);
    int move_camera_up = glfwGetKey(window, GLFW_KEY_W);
    int move_camera_down = glfwGetKey(window, GLFW_KEY_S);
    int move_camera_forward = glfwGetKey(window, GLFW_KEY_SPACE);
    int move_camera_backward = glfwGetKey(window, GLFW_KEY_BACKSPACE);

    // Teleportation to fixed views
    int move_camera_to_position_1 = glfwGetKey(window, GLFW_KEY_H);
    int move_camera_to_position_2 = glfwGetKey(window, GLFW_KEY_J);
    int move_camera_to_position_3 = glfwGetKey(window, GLFW_KEY_K);
    int move_camera_to_default = glfwGetKey(window, GLFW_KEY_L);

    // Moving object
    int move_object_left = glfwGetKey(window, GLFW_KEY_LEFT);
    int move_object_right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int move_object_up = glfwGetKey(window, GLFW_KEY_UP);
    int move_object_down = glfwGetKey(window, GLFW_KEY_DOWN);
    int move_object_forward = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    int move_object_backward = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);

    // Spinning object about x-axis
    int spin_object_about_x = glfwGetKey(window, GLFW_KEY_N);
    int unspin_object_about_x = glfwGetKey(window, GLFW_KEY_M);

    // Spin camera about object
    int spin_camera = glfwGetKey(window, GLFW_KEY_C);
    if(spin_camera){
        spin_camera_about_object = 1;
    }

    // Camera translating
    if(move_camera_left){
        camera_shift(-1 * camera_speed * i_cap);
    } 
    if(move_camera_right) {
        camera_shift(1 * camera_speed * i_cap);
    } 
    if(move_camera_up) {
        camera_shift(1 * camera_speed * j_cap);
    } 
    if(move_camera_down){
        camera_shift(-1 * camera_speed * j_cap);
    } 
    if(move_camera_backward){
        camera_shift(1 * camera_speed * k_cap);
    }
    if(move_camera_forward){
        camera_shift(-1 * camera_speed * k_cap);
    }

    // Teleportation
    if(move_camera_to_position_1){
        camera_eye = camera_fixed_position1_eye;
        camera_target = camera_fixed_position1_target;
        camera_front_vector = camera_target - camera_eye;
        camera_position = camera_eye;
    }
    if(move_camera_to_position_2){
        camera_eye = camera_fixed_position2_eye;
        camera_target = camera_fixed_position2_target;
        camera_front_vector = camera_target - camera_eye;
        camera_position = camera_eye;
    }
    if(move_camera_to_position_3){
        camera_eye = camera_fixed_position3_eye;
        camera_target = camera_fixed_position3_target;
        camera_front_vector = camera_target - camera_eye;
        camera_position = camera_eye;
    }
    if(move_camera_to_default){
        camera_eye = glm::vec3(0.0f, 0.0f, 5.0f);
        camera_position = camera_eye;
        camera_front_vector = glm::vec3(0.0f, 0.0f, -1.0f);
        camera_target = camera_position + camera_front_vector;
        spin_camera_about_object = 0;        
    }

    // Display this figure on key press
    if(get_dec_prism){
        current_fig_displaying = 0;
    }
    if(get_hex_dipyr){
        current_fig_displaying = 1;
    }
    if(get_undec_pyr){
        current_fig_displaying = 2;
    }

    // Spin the object about x axis
    if(spin_object_about_x){
        if(current_fig_displaying == 0){
            spin_object<DecagonalPrism>(dec_prism);
        } else if(current_fig_displaying == 1){
            spin_object<HexagonalDiPyramid>(hex_dipyr);
        } else {
            spin_object<UndecagonalPyramid>(undec_pyr);
        }
    }
    // Stop spinning of the object about x axis
    if(unspin_object_about_x){
        if(current_fig_displaying == 0){
            unspin_object<DecagonalPrism>(dec_prism);
        } else if(current_fig_displaying == 1){
            unspin_object<HexagonalDiPyramid>(hex_dipyr);
        } else {
            unspin_object<UndecagonalPyramid>(undec_pyr);
        }
    }

    // Translate object based on key press
    if(move_object_forward){
        if(current_fig_displaying == 0){
            translate_object<DecagonalPrism>(dec_prism, 2, object_speed);
        } else if(current_fig_displaying == 1){
            translate_object<HexagonalDiPyramid>(hex_dipyr, 2, object_speed);
        } else {
            translate_object<UndecagonalPyramid>(undec_pyr, 2, object_speed);
        }
    }
    if(move_object_backward){
        if(current_fig_displaying == 0){
            translate_object<DecagonalPrism>(dec_prism, 2, -1*object_speed);
        } else if(current_fig_displaying == 1){
            translate_object<HexagonalDiPyramid>(hex_dipyr, 2, -1*object_speed);
        } else {
            translate_object<UndecagonalPyramid>(undec_pyr, 2, -1*object_speed);
        }
    }
    if(move_object_up){
        if(current_fig_displaying == 0){
            translate_object<DecagonalPrism>(dec_prism, 1, object_speed);
        } else if(current_fig_displaying == 1){
            translate_object<HexagonalDiPyramid>(hex_dipyr, 1, object_speed);
        } else {
            translate_object<UndecagonalPyramid>(undec_pyr, 1, object_speed);
        }
    }
    if(move_object_down){
        if(current_fig_displaying == 0){
            translate_object<DecagonalPrism>(dec_prism, 1, -1*object_speed);
        } else if(current_fig_displaying == 1){
            translate_object<HexagonalDiPyramid>(hex_dipyr, 1, -1*object_speed);
        } else {
            translate_object<UndecagonalPyramid>(undec_pyr, 1, -1*object_speed);
        }
    }
    if(move_object_right){
        if(current_fig_displaying == 0){
            translate_object<DecagonalPrism>(dec_prism, 0, object_speed);
        } else if(current_fig_displaying == 1){
            translate_object<HexagonalDiPyramid>(hex_dipyr, 0, object_speed);
        } else {
            translate_object<UndecagonalPyramid>(undec_pyr, 0, object_speed);
        }
    }
    if(move_object_left){
        if(current_fig_displaying == 0){
            translate_object<DecagonalPrism>(dec_prism, 0, -1*object_speed);
        } else if(current_fig_displaying == 1){
            translate_object<HexagonalDiPyramid>(hex_dipyr, 0, -1*object_speed);
        } else {
            translate_object<UndecagonalPyramid>(undec_pyr, 0, -1*object_speed);
        }
    }
}

void tick_elements() {

    if(current_fig_displaying == 0){
        dec_prism.tick();
    } else if(current_fig_displaying == 1){
        hex_dipyr.tick();
    } else {
        undec_pyr.tick();
    }
    camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    hex_dipyr = HexagonalDiPyramid(0, 0);
    dec_prism = DecagonalPrism(0, 0);
    undec_pyr = UndecagonalPyramid(0, 0);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
