#include "camera.h"

#define _USE_MATH_DEFINES

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3 &position, const glm::vec2 &orientation) : m_position(position), m_orientation(orientation) {
    // Initialisation des attributs de la classe
}

glm::mat4 Camera::getFirstPersonViewMatrix() {
    // Calcul et retour de la matrice de vue en première personne
    // à partir des attributs de la classe
    glm::mat4 camera = glm::rotate(glm::mat4(1.0f), -glm::atan(m_orientation.y / m_orientation.x), glm::vec3(0, 1, 0));
    glm::mat4 view = glm::translate(camera, -m_position);
    return view;
}

glm::mat4 Camera::getThirdPersonViewMatrix() {
    // TODO: Calcul et retour de la matrice de vue en troisième personne
    // à partir des attributs de la classe
    glm::mat4 look = glm::lookAt(glm::vec3(2.0f),m_position,glm::vec3(0, 1, 0));
    glm::mat4 cameraX = glm::rotate(glm::mat4(1.0f),  m_orientation.x, glm::vec3(0, 1, 0));
    glm::mat4 cameraY = glm::rotate(glm::mat4(1.0f), m_orientation.y, glm::vec3(1, 0, 0));
//    glm::mat4 view = glm::translate(glm::vec3(1.0f) -m_position);
    return look * cameraX * cameraY;
}

