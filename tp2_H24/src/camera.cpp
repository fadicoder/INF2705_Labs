#include "camera.h"

#define _USE_MATH_DEFINES

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(const glm::vec3 &position, const glm::vec2 &orientation) : m_position(position), m_orientation(orientation) {
    // Initialisation des attributs de la classe
}

glm::mat4 Camera::getFirstPersonViewMatrix() {
    // Calcul et retour de la matrice de vue en première personne
    // à partir des attributs de la classe
    glm::mat4 view = glm::rotate(glm::mat4(1.0f), -m_orientation.x, glm::vec3(1, 0, 0));
    view = glm::rotate(view, -m_orientation.y, glm::vec3(0, 1, 0));
    view = glm::translate(view, -m_position);
    return view;
}

glm::mat4 Camera::getThirdPersonViewMatrix() {
    // TODO: Calcul et retour de la matrice de vue en troisième personne
    // à partir des attributs de la classe
    float x = 6 * glm::cos(m_orientation.x) * glm::cos(m_orientation.y);
    float y = 6 * glm::sin(m_orientation.x) * glm::cos(m_orientation.y);
    float z = 6 * glm::sin(m_orientation.y);
    glm::vec3 cameraPosition = m_position - glm::vec3(x, y, z);
    return glm::lookAt(cameraPosition,m_position,glm::vec3(0, 1, 0));
}

