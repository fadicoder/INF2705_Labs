
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <array>
#include <cmath>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window.h"
#include "shader_program.h"
#include "vertices_data.h"
#include "shapes.h"
#include "camera.h"
#include "model.h"
#include "utils.h"
#include "textures.h"


static std::random_device rd;  // Will be used to obtain a seed for the random number engine
static std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()


void resizeWindowOnChange(Window &w) {
    if (w.shouldResize())
        glViewport(0, 0, w.getWidth(), w.getHeight());
}

void updateTransformation(Window &w, Camera &camera, glm::vec3 &position, glm::vec2 &orientation, float &angleDeg,
                          const GLint MATRIX_LOCATION) {
    // Calcul des matrices et envoyer une matrice résultante mvp au shader.
    // Utiliser glm pour les calculs de matrices.
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angleDeg), glm::vec3(0.1f, 1.0f, 0.1f));

    glm::mat4 view = camera.getThirdPersonViewMatrix();

    glm::mat4 projection = glm::perspective(70.0f, (float) (w.getWidth() / w.getHeight()), 0.1f, 200.0f);

    glm::mat4 transformation = projection * view * model;

    glUniformMatrix4fv(MATRIX_LOCATION, 1.0f, GL_FALSE, (GLfloat *) &transformation);
}

void updateModelMatrix(Window &w, Camera &camera, glm::mat4 &model, const GLint MATRIX_LOCATION) {
    // Calcul des matrices et envoyer une matrice résultante mvp au shader.
    // Utiliser glm pour les calculs de matrices.

    glm::mat4 view = camera.getThirdPersonViewMatrix();

    glm::mat4 projection = glm::perspective(70.0f, (float) (w.getWidth() / w.getHeight()), 0.1f, 200.0f);

    glm::mat4 transformation = projection * view * model;

    glUniformMatrix4fv(MATRIX_LOCATION, 1.0f, GL_FALSE, (GLfloat *) &transformation);
}

void move(Window &w, glm::vec3 &position, glm::vec2 &orientation) {
    const glm::vec3 orientationVector = glm::vec3(glm::cos(orientation.y), 0, glm::sin(orientation.y));
    const glm::vec3 perpendicularVector = glm::vec3(-glm::sin(orientation.y), 0, glm::cos(orientation.y));
    if (w.getKeyHold(Window::Key::W)) {
        position += orientationVector;
    } else if (w.getKeyHold(Window::Key::S)) {
        position -= orientationVector;
    } else if (w.getKeyHold(Window::Key::A)) {
        position -= perpendicularVector;
    } else if (w.getKeyHold(Window::Key::D)) {
        position += perpendicularVector;
    }
}

void look(Window &w, glm::vec2 &orientation) {
    const float PITCH_MOVE = 0.05f;
    const float YAW_MOVE = 0.05f;
    if (w.getKeyHold(Window::Key::UP)) {
        orientation.x += PITCH_MOVE;
    } else if (w.getKeyHold(Window::Key::DOWN)) {
        orientation.x -= PITCH_MOVE;
    } else if (w.getKeyHold(Window::Key::LEFT)) {
        orientation.y -= YAW_MOVE;
    } else if (w.getKeyHold(Window::Key::RIGHT)) {
        orientation.y += YAW_MOVE;
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

glm::mat4 getConstantScale(glm::mat4 transform, float scale) {
    return glm::scale(transform, glm::vec3(scale, scale, scale));
}


glm::mat4 getRandomScale(glm::mat4 transform) {
    std::uniform_real_distribution<> rnd(0.7f, 1.3f);
    float scale = rnd(gen);
    return getConstantScale(transform, scale);
}



glm::mat4 getRandomRotation(glm::mat4 transform) {
    std::uniform_real_distribution<> rnd(0, 2 * M_PI);
    float rot = rnd(gen);
    return glm::rotate(transform, rot, glm::vec3(0.f,1.f,0.f));
}

void drawSingleModel(Window &w, Camera &camera, Model& model, Texture2D& tex, glm::mat4& transform, const GLint MATRIX_LOCATION){
    updateModelMatrix(w, camera, transform, MATRIX_LOCATION);
    tex.use();
    model.draw();
    Texture2D::unuse();
}

void drawGroup(
        Window &w,
        Camera &camera,
        const GLint MATRIX_LOCATION,
        Model &tree, Texture2D& treeTex, glm::mat4 &treeTransform,
        Model &rock, Texture2D& rockTex, glm::mat4 &rockTransform,
        Model &mushroom,  Texture2D& shroomTex, glm::mat4 &shroomTransform) {
    drawSingleModel(w, camera, tree, treeTex, treeTransform, MATRIX_LOCATION);
    drawSingleModel(w, camera, rock, rockTex, rockTransform, MATRIX_LOCATION);
    drawSingleModel(w, camera, mushroom, shroomTex, shroomTransform, MATRIX_LOCATION);
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

    // HUD
    BasicShapeElements redSquare(colorSquareVerticesReduced, sizeof(colorSquareVerticesReduced), indexes, sizeof(indexes));
    redSquare.enableAttribute(0, 3, sizeof(float)*7, 0);
    redSquare.enableAttribute(1, 4, sizeof(float)*7, (sizeof(float)*3));
    Texture2D heartTex("../textures/heart.png", GL_CLAMP_TO_BORDER);

    BasicShapeElements ground(groundVertices, sizeof(groundVertices), groundIndexes,
                              sizeof(groundIndexes));
    ground.enableAttribute(0, 3, sizeof(float) * 5, 0);
    ground.enableAttribute(1, 2, sizeof(float) * 5, (sizeof(float) * 3));
    Texture2D groundTex("../textures/groundSeamless.jpg", GL_REPEAT);

    BasicShapeElements river(riverVertices, sizeof(riverVertices), riverIndexes,
                             sizeof(riverIndexes));
    river.enableAttribute(0, 3, sizeof(float) * 5, 0);
    river.enableAttribute(1, 2, sizeof(float) * 5, (sizeof(float) * 3));
    Texture2D riverTex("../textures/waterSeamless.jpg", GL_REPEAT);
    const GLint MATRIX_LOCATION = transformProgram.getUniformLoc("mvp");

    Model suzanne("../models/suzanne.obj");
    glm::vec3 position = glm::vec3(0, 1, 0);
    glm::vec2 orientation = glm::vec2(0, 0);
    glm::mat4 suzanneTransform = getConstantScale(glm::translate(glm::mat4(1.0f), {0, -1, 0}), 0.6f);
    const GLint MODEL_MATRIX_LOCATION = modelProgram.getUniformLoc("mvp");

    Texture2D texSuzanne("../models/suzanneTexture.png", GL_CLAMP_TO_EDGE);
    const GLint TEX_UNIT_LOCATION = modelProgram.getUniformLoc("texSampler");
    glUniform1i(TEX_UNIT_LOCATION, 0);

    Model tree("../models/tree.obj");
    Texture2D texTree("../models/treeTexture.png", GL_CLAMP_TO_BORDER);
    Model rock("../models/rock.obj");
    Texture2D texRock("../models/rockTexture.png", GL_CLAMP_TO_BORDER);
    Model mushroom("../models/mushroom.obj");
    Texture2D texShroom("../models/mushroomTexture.png", GL_CLAMP_TO_BORDER);
    const int N_ROWS = 7;
    const int N_GROUPS = N_ROWS * N_ROWS;

    glm::mat4 groupsTransform[N_GROUPS];
    glm::mat4 treeTransform[N_GROUPS];
    glm::mat4 rockTransform[N_GROUPS];
    glm::mat4 shroomTransform[N_GROUPS];
    for (int i = 0; i < N_GROUPS; i++) {
        float x, z;
        getGroupRandomPos(i, 1, x, z);
        glm::vec3 randomPos = glm::vec3(x, -1.0f, z);
        groupsTransform[i] = getRandomScale(getRandomRotation(glm::translate(glm::mat4(1.0f), randomPos)));
        treeTransform[i] = getRandomScale(getRandomRotation(glm::translate(glm::mat4(1.0f), randomPos)));

        auto rockPos = randomPos;
        rockPos.y += 0.2;
        rockPos.x -= 0.5;
        rockPos.z += 0.1;
        rockTransform[i] = getConstantScale(getRandomRotation(glm::translate(glm::mat4(1.0f), rockPos)), 0.3f);

        auto shroomPos = randomPos;
        shroomPos.x += 0.7;
        shroomPos.z += 0.7;
        shroomTransform[i] = getConstantScale(getRandomRotation(glm::translate(glm::mat4(1.0f), shroomPos)), 0.05);
    }

    Camera camera(position, orientation);

    // Partie 1: Donner une couleur de remplissage aux fonds.
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Partie 2: Activer le depth test.
    glEnable(GL_DEPTH_TEST);

    bool isRunning = true;
    while (isRunning) {
        resizeWindowOnChange(w);

        // Nettoyer les tampons appropriées.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        move(w, position, orientation);
        look(w, orientation);

        // 2D elements
        // Mets la transformation de la caméra
        transformProgram.use();
        updateTransformation(w, camera, position, orientation, angleDeg, MATRIX_LOCATION);

        // HUD
//        heartTex.use();
//        redSquare.draw(GL_TRIANGLES, 6);
//        Texture2D::unuse();

        // Sol
        groundTex.use();
        ground.draw(GL_TRIANGLES, 6);
        Texture2D::unuse();

        // Riviere
        riverTex.use();
        river.draw(GL_TRIANGLES, 6);
        Texture2D::unuse();

        // 3D elements
        modelProgram.use();
        updateTransformation(w, camera, position, orientation, angleDeg, MODEL_MATRIX_LOCATION);

        // Suzanne
        updateModelMatrix(w, camera, suzanneTransform, MODEL_MATRIX_LOCATION);
        texSuzanne.use();
        suzanne.draw();
        Texture2D::unuse();

        // Vegetation
        for (int i = 0; i < N_GROUPS; ++i) {
            drawGroup(w, camera, MODEL_MATRIX_LOCATION, tree, texTree, treeTransform[i], rock, texRock, rockTransform[i],
                      mushroom, texShroom, shroomTransform[i]);
        }

        GL_CHECK_ERROR;

        // Update la fenetre
        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKeyPress(Window::Key::ESC);
    }

    return 0;
}