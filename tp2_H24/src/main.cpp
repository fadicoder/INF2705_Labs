
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <array>
#include <cmath>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"
#include "shader_program.h"
#include "vertices_data.h"
#include "shapes.h"
#include "camera.h"
#include "model.h"
#include "utils.h"


#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)

static std::random_device rd;  // Will be used to obtain a seed for the random number engine
static std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()


void resizeWindowOnChange(Window &w) {
    if (w.shouldResize())
        glViewport(0, 0, w.getWidth(), w.getHeight());
}

void updateTransformation(Window &w, Camera &camera, glm::vec3 &position, glm::vec2 &orientation, float& angleDeg,
                          const GLint MATRIX_LOCATION) {
    // Calcul des matrices et envoyer une matrice résultante mvp au shader.
    // Utiliser glm pour les calculs de matrices.
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angleDeg), glm::vec3(0.1f, 1.0f, 0.1f));

    glm::mat4 view = camera.getThirdPersonViewMatrix();

    glm::mat4 projection = glm::perspective(70.0f, (float) (w.getWidth() / w.getHeight()), 0.1f, 200.0f);

    glm::mat4 transformation = projection * view  * model;

    glUniformMatrix4fv(MATRIX_LOCATION, 1.0f, GL_FALSE, (GLfloat *) &transformation);
}

void reactToKeys(Window &w, glm::vec3 &position, glm::vec2 &orientation){
    const float Y_MOVE = 0.1f;
    const float X_MOVE = 0.1f;
    if(w.getKeyPress(Window::Key::W)){
        orientation.y -= Y_MOVE;
    }else if(w.getKeyPress(Window::Key::S)){
        orientation.y += Y_MOVE;
    } else if(w.getKeyPress(Window::Key::A)){
        orientation.x -= X_MOVE;
    } else if(w.getKeyPress(Window::Key::D)){
        orientation.x += X_MOVE;
    }
}


void checkGLError(const char *file, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "GL_ERROR, File " << file << " (Line " << line << "): ";
        switch (error) {
            case GL_INVALID_ENUM:
                std::cerr << "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                std::cerr << "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                std::cerr << "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                std::cerr << "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                std::cerr << "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                std::cerr << "GL_STACK_OVERFLOW";
                break;
            default:
                std::cerr << "Unknown gl error occured!";
                break;
        }
        std::cerr << std::endl;
    }
}

void printGLInfo() {
    std::cout << "OpenGL info:" << std::endl;
    std::cout << "    Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "    Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "    Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "    Shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


std::string readFile(const char *path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

//std::array<glm::vec3, 7> getGroupRandomPos(int count = 7){
//    std::uniform_real_distribution<> rnd(-60.0f, 60.0f);
//
//    std::array<glm::vec3, 7> positions{
//            glm::vec3(rnd(gen), 0.0f, rnd(gen)),
//            glm::vec3(rnd(gen), 0.0f, rnd(gen)),
//            glm::vec3(-rnd(gen), 0.0f, -rnd(gen)),
//            glm::vec3(-rnd(gen), 0.0f, -rnd(gen)),
//            glm::vec3(rnd(gen), 0.0f, -rnd(gen)),
//            glm::vec3(-rnd(gen), 0.0f, rnd(gen)),
//            glm::vec3(rnd(gen), 0.0f, -rnd(gen)),
//    };
//    return positions;
//}


float getRandomScale(){
    std::uniform_real_distribution<> rnd(0.7f, 1.3f);
    return rnd(gen);
}

float getRandomRotation(){
    std::uniform_real_distribution<> rnd(0, 2*M_PI);
    return rnd(gen);
}

int main(int argc, char *argv[]) {
    Window w;
    if (!w.init())
        return -1;

    GLenum rev = glewInit();
    if (rev != GLEW_OK) {
        std::cout << "Could not initialize glew! GLEW_Error: " << glewGetErrorString(rev) << std::endl;
        return -2;
    }

    printGLInfo();

    // Transform program
    ShaderProgram transformProgram;
    {
        std::string str = readFile("shaders/transform.fs.glsl");
        std::string vstr = readFile("shaders/transform.vs.glsl");

        Shader fragShader(GL_FRAGMENT_SHADER, str.c_str());
        Shader vertShader(GL_VERTEX_SHADER, vstr.c_str());

        transformProgram.attachShader(vertShader);
        transformProgram.attachShader(fragShader);
        transformProgram.link();
    }

    // Transform program
    ShaderProgram modelProgram;
    {
        std::string str = readFile("shaders/model.fs.glsl");
        std::string vstr = readFile("shaders/model.vs.glsl");

        Shader fragShader(GL_FRAGMENT_SHADER, str.c_str());
        Shader vertShader(GL_VERTEX_SHADER, vstr.c_str());

        modelProgram.attachShader(vertShader);
        modelProgram.attachShader(fragShader);
        modelProgram.link();
    }
    float angleDeg = 0.0f;

//    BasicShapeElements shape6(cubeVertices, sizeof(cubeVertices), reinterpret_cast<const GLuint *>(cubeIndexes),
//                              sizeof(cubeIndexes));
//    shape6.enableAttribute(0, 3, sizeof(float) * 6, 0);
//    shape6.enableAttribute(1, 3, sizeof(float) * 6, (sizeof(float) * 3));
//    const GLint MATRIX_LOCATION = transformProgram.getUniformLoc("mvp");
//    glm::vec3 position = glm::vec3(0, 0, 0);
//    glm::vec2 orientation = glm::vec2(0, 0);

    BasicShapeElements ground(groundVertices, sizeof(groundVertices), reinterpret_cast<const GLuint *>(cubeIndexes),
                              sizeof(cubeIndexes));
    ground.enableAttribute(0, 3, sizeof(float) * 6, 0);
    ground.enableAttribute(1, 3, sizeof(float) * 6, (sizeof(float) * 3));
    const GLint MATRIX_LOCATION = transformProgram.getUniformLoc("mvp");
    glm::vec3 position = glm::vec3(0, 1, 0);
    glm::vec2 orientation = glm::vec2(0, 0);


    Model tree("../models/tree.obj");
    const GLint MODEL_MATRIX_LOCATION = modelProgram.getUniformLoc("mvp");
    const GLint COLOR_LOCATION = modelProgram.getUniformLoc("color");
    GLfloat color[3] = { 1.0f, 0.0f, 0.0f };
    glUniform3fv(COLOR_LOCATION, 1.0f, color);
    glm::mat4 treeTransformMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(MODEL_MATRIX_LOCATION, 1.0f, GL_FALSE, (GLfloat*) &treeTransformMatrix);
//    Model rock("../models/rock.obj");
//    Model mushroom("../models/mushroom.obj");
//    Model[] models[] = {
//            {tree},
//            {tree},
//            {tree},
//            {tree},
//            {tree},
//            {tree},
//            {tree},
//    };


    Camera camera(position, orientation);

    // Partie 1: Donner une couleur de remplissage aux fonds.
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Partie 2: Activer le depth test.
    glEnable(GL_DEPTH_TEST);

    bool isRunning = true;
    transformProgram.use();
//    modelProgram.use();
    while (isRunning) {
        resizeWindowOnChange(w);

        // Nettoyer les tampons appropriées.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        reactToKeys(w, position, orientation);

        // Update la transformation du shape6
        updateTransformation(w, camera, position, orientation, angleDeg, MATRIX_LOCATION);

//         Dessine l'image

//        shape6.draw(GL_TRIANGLES, 36);

//        modelProgram.use();
//        tree.draw();
//        GL_CHECK_ERROR;

        transformProgram.use();
        ground.draw(GL_TRIANGLES, 36);
        GL_CHECK_ERROR;

        // Update la fenetre
        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKeyPress(Window::Key::ESC);
    }

    return 0;
}