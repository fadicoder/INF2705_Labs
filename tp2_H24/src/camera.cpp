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
    glm::mat4 view = glm::rotate(glm::mat4(1.0f), -this->m_orientation.x, glm::vec3(1, 0, 0));
    view = glm::rotate(glm::mat4(1.0f), -this->m_orientation.y, glm::vec3(0, 1, 0));
    view = glm::translate(view, -m_position);
    return view;
}

glm::mat4 Camera::getThirdPersonViewMatrix() {
    // TODO: Calcul et retour de la matrice de vue en troisième personne
    // à partir des attributs de la classe
    glm::vec3 cameraPosition = m_position - glm::vec3(6 * glm::cos(m_orientation.x), 6 * glm::sin(m_orientation.y), 6);
    std::cout << "Orientation: " << m_orientation.x << " : " << m_orientation.y << std::endl;
    std::cout << "cameraPosition: " << cameraPosition.x << " : " << cameraPosition.y << " : " << cameraPosition.z << std::endl;
    std::cout << "Distance: " << glm::distance(m_position, cameraPosition) << std::endl;
    glm::mat4 look = glm::lookAt(cameraPosition,m_position,glm::vec3(0, 1, 0));
    // glm::mat4 cameraX = glm::rotate(glm::mat4(1.0f),  -m_orientation.x, glm::vec3(0, 1, 0));
    // glm::mat4 cameraY = glm::rotate(glm::mat4(1.0f), -m_orientation.y, glm::vec3(1, 0, 0));
//    glm::mat4 view = glm::translate(glm::vec3(1.0f) -m_position);
    // return look * cameraX * cameraY;
    return look;
}

