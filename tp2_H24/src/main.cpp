
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window.h"
#include "shader_program.h"
#include "vertices_data.h"
#include "shapes.h"
#include "camera.h"


#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)

void resizeWindowOnChange(Window &w) {
    if (w.shouldResize())
        glViewport(0, 0, w.getWidth(), w.getHeight());
}

void updateCubeTransformation(Window &w, Camera &camera, glm::vec3 &position, glm::vec2 &orientation, float angleDeg,
                              const GLint MATRIX_LOCATION) {// Calcul des matrices et envoyer une matrice résultante mvp au shader.
    angleDeg += 0.5f;

    // Utiliser glm pour les calculs de matrices.
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angleDeg), glm::vec3(0.1f, 1.0f, 0.1f));

    glm::mat4 view = camera.getThirdPersonViewMatrix();

    glm::mat4 projection = glm::perspective(70.0f, (float) (w.getWidth() / w.getHeight()), 0.1f, 10.0f);

    glm::mat4 transformation = projection * view  * model;

    glUniformMatrix4fv(MATRIX_LOCATION, 1.0f, GL_FALSE, (GLfloat *) &transformation);
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
    float angleDeg = 0.0f;

    BasicShapeElements shape6(cubeVertices, sizeof(cubeVertices), reinterpret_cast<const GLuint *>(cubeIndexes),
                              sizeof(cubeIndexes));
    shape6.enableAttribute(0, 3, sizeof(float) * 6, 0);
    shape6.enableAttribute(1, 3, sizeof(float) * 6, (sizeof(float) * 3));
    const GLint MATRIX_LOCATION = transformProgram.getUniformLoc("mvp");
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec2 orientation = glm::vec2(0, 0);


    Camera camera(position, orientation);

    // Partie 1: Donner une couleur de remplissage aux fonds.
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Partie 2: Activer le depth test.
    glEnable(GL_DEPTH_TEST);

    bool isRunning = true;
    transformProgram.use();
    while (isRunning) {
        resizeWindowOnChange(w);

        // Nettoyer les tampons appropriées.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update la transformation du shape6
        updateCubeTransformation(w, camera, position, orientation, angleDeg, MATRIX_LOCATION);

        // Dessine l'image
        shape6.draw(GL_TRIANGLES, 36);
        GL_CHECK_ERROR;

        // Update la fenetre
        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKeyPress(Window::Key::ESC);
    }

    return 0;
}