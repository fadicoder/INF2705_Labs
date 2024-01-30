
#include <stdio.h>
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


void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)

void checkGLError(const char *file, int line);

void changeRGB(GLfloat *color);

void changePos(GLfloat *pos, float &cx, float &cy, float &dx, float &dy);

std::string readFile(const char *path);

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

    // Partie 1: Instancier les shader programs ici.
    ShaderProgram baseShaderProgram;
    {
        // Les accolades vont permettre de détruire le code des shaders plus rapidement
        // Vous devez lire le code des shaders dans "shaders/"
        // avec la fonction "readFile".
        std::string str = readFile("shaders/basic.fs.glsl");
        std::string vstr = readFile("shaders/basic.vs.glsl");

        // Vous pouvez par la suite instancier vos shaders, les attacher et les lier
        // au programme.
        Shader fragShaderBase = Shader(GL_FRAGMENT_SHADER, str.c_str());
        Shader vertShaderBase = Shader(GL_VERTEX_SHADER, vstr.c_str());

        baseShaderProgram.attachShader(vertShaderBase);
        baseShaderProgram.attachShader(fragShaderBase);
        baseShaderProgram.link();
    }

    // ... color;
    ShaderProgram colorProgram;
    {
        std::string str = readFile("shaders/color.fs.glsl");
        std::string vstr = readFile("shaders/color.vs.glsl");

        Shader fragShader(GL_FRAGMENT_SHADER, str.c_str());
        Shader vertShader(GL_VERTEX_SHADER, vstr.c_str());

        colorProgram.attachShader(fragShader);
        colorProgram.attachShader(vertShader);
        colorProgram.link();
    }

    // TODO Partie 2: Shader program de transformation.
    // ... transform;
    // ... location;
//    ShaderProgram transformProgram;
    {
//         std::string str = readFile("shaders/transform.fs.glsl");
//         std::string vstr = readFile("shaders/transform.vs.glsl");

//         Shader fragShader(GL_FRAGMENT_SHADER, str.c_str());
//         Shader vertShader(GL_VERTEX_SHADER, vstr.c_str());

//         transformProgram.attachShader(vertShader);
//         transformProgram.attachShader(fragShader);
//         transformProgram.link();
    }

    // Variables pour la mise à jour, ne pas modifier.
    float cx = 0, cy = 0;
    float dx = 0.019;
    float dy = 0.0128;

    float angleDeg = 0.0f;

    // Tableau non constant de la couleur
    GLfloat onlyColorTriVertices[] = {
            // Partie 1: Remplir adéquatement le tableau.
            // Vous pouvez expérimenter avec une couleur uniforme
            // de votre choix ou plusieurs différentes en chaque points.
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
    };

    // Partie 1: Instancier vos formes ici.
    BasicShapeArrays shape0(triVertices, sizeof(triVertices));
    shape0.enableAttribute(0, 3, 0, 0);

    BasicShapeArrays shape1(squareVertices, sizeof(squareVertices));
    shape1.enableAttribute(0, 3, 0, 0);

    BasicShapeArrays shape2(colorTriVertices, sizeof(colorTriVertices));
    shape2.enableAttribute(0, 3, sizeof(float)*7, 0);
    shape2.enableAttribute(1, 4, sizeof(float)*7, (sizeof(float)*3));

    BasicShapeArrays shape3(colorSquareVertices, sizeof(colorSquareVertices));
    shape3.enableAttribute(0, 3, sizeof(float)*7, 0);
    shape3.enableAttribute(1, 4, sizeof(float)*7, (sizeof(float)*3));

    BasicShapeMultipleArrays shape4(triVertices, sizeof(triVertices), onlyColorTriVertices, sizeof(onlyColorTriVertices));
    shape4.enablePosAttribute(0, 3, 0, 0);
    shape4.enableColorAttribute(1, 3, 0, 0);
    GLfloat* posPtr = shape4.mapPosData();

    BasicShapeElements shape5(colorSquareVerticesReduced, sizeof(colorSquareVerticesReduced), indexes, sizeof(indexes));
    shape5.enableAttribute(0, 3, sizeof(float)*7, 0);
    shape5.enableAttribute(1, 4, sizeof(float)*7, (sizeof(float)*3));


    // TODO Partie 2: Instancier le cube ici.
//    BasicShapeElements cubeShape(cubeVertices, sizeof(cubeVertices), cubeIndexes, sizeof(cubeIndexes));
//    cubeShape.enableAttribute(0, 3, 0, 0);
//    cubeShape.enableAttribute(0, 3, 0, 12);

    // Partie 1: Donner une couleur de remplissage aux fonds.
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Partie 2: Activer le depth test.
    glEnable(GL_DEPTH_TEST);

    int selectShape = 0;
    bool isRunning = true;
    while (isRunning) {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());

        // Partie 1: Nettoyer les tampons appropriées.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (w.getKey(Window::Key::T)) {
            selectShape = ++selectShape < 7 ? selectShape : 0;
            std::cout << "Selected shape: " << selectShape << std::endl;
        }
        
        // Partie 1: Mise à jour des données du triangle
        changeRGB(&onlyColorTriVertices[0]);
        changeRGB(&onlyColorTriVertices[3]);
        changeRGB(&onlyColorTriVertices[6]);
        
        changePos(posPtr, cx, cy, dx, dy);
        shape4.updateColorData(onlyColorTriVertices, sizeof(onlyColorTriVertices));

        // Partie 1: Utiliser le bon shader programme selon la forme.
        // N'hésiter pas à utiliser le fallthrough du switch case.
        switch (selectShape) {
            case 0:
            case 1:
                baseShaderProgram.use();
                break;
            case 2:
            case 3:
            case 4:
            case 5:
                colorProgram.use();
                break;
            case 6:
//                transformProgram.use();
            default:
                break;
        }

        // TODO Partie 2: Calcul des matrices et envoyer une matrice résultante mvp au shader.
        if (selectShape == 6) {
            angleDeg += 0.5f;
            // Utiliser glm pour les calculs de matrices.
            // Dans lemain, il va falloir utiliser la méthodegetUniformLocpour pouvoir connaître la location du uniform dela matrice
//            glm::mat4 matrix = glGetUniformfv(transformProgram transformProgram.getUniformLoc("mvp"));

//            matrix = glm::rotate(matrix, angleDeg, glm::vec3(0.1, 1.0, 0.1));
//            matrix = glm::translate(matrix, glm::vec3(0, 0.5, 2));
//            matrix = glm::scale(matrix, glm::vec3(70, 0.1, 10.0));
//            glUniformMatrix4fv(matrix);
        }

        // Partie 1: Dessiner la forme sélectionnée.
        switch (selectShape) {
            case 0:
                shape0.draw(GL_TRIANGLES, 3);
                break;
            case 1:
                shape1.draw(GL_TRIANGLES, 6);
                break;
            case 2:
                shape2.draw(GL_TRIANGLES, 3);
                break;
            case 3:
                shape3.draw(GL_TRIANGLES, 6);
                break;
            case 4:
                shape4.draw(GL_TRIANGLES, 3);
                break;
            case 5:
                shape5.draw(GL_TRIANGLES, 6);
                break;
            case 6:
//                cubeShape.draw(GL_TRIANGLES, 36);
                break;
            default:
                break;
        }
        GL_CHECK_ERROR;

        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKey(Window::Key::ESC);
    }

    return 0;
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


void changeRGB(GLfloat *color) {
    unsigned char r = color[0] * 255;
    unsigned char g = color[1] * 255;
    unsigned char b = color[2] * 255;

    if (r > 0 && b == 0) {
        r--;
        g++;
    }
    if (g > 0 && r == 0) {
        g--;
        b++;
    }
    if (b > 0 && g == 0) {
        r++;
        b--;
    }
    color[0] = r / 255.0f;
    color[1] = g / 255.0f;
    color[2] = b / 255.0f;
}

void changePos(GLfloat *pos, float &cx, float &cy, float &dx, float &dy) {
    if ((cx < -1 && dx < 0) || (cx > 1 && dx > 0))
        dx = -dx;
    pos[0] += dx;
    pos[3] += dx;
    pos[6] += dx;
    cx += dx;
    if ((cy < -1 && dy < 0) || (cy > 1 && dy > 0))
        dy = -dy;
    pos[1] += dy;
    pos[4] += dy;
    pos[7] += dy;
    cy += dy;
}


std::string readFile(const char *path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
