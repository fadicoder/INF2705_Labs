#include "shapes.h"

BasicShapeArrays::BasicShapeArrays(const GLfloat *data, GLsizeiptr byteSize) {
    // Partie 1: Générer et bind le vao de la forme.
    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);

    // Partie 1: Générer et bind le vbo de la forme.
    // Allouer l'espace nécessaire dans le mode approprié
    // et envoyer les données au gpu.
    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

    glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

BasicShapeArrays::~BasicShapeArrays() {
    // Partie 1: Supprimer la mémoire de l'objet.
    // Assurez-vous que les ressources ne soient pas utilisées
    // pendant la suppression.
    glDeleteBuffers(1, &this->m_vbo);
    glDeleteVertexArrays(1, &this->m_vao);
}

void BasicShapeArrays::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
    // Partie 1: Activer un attribut et l'attacher correctement au state du vao.
    glBindVertexArray(this->m_vao);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (GLvoid *) offset);
    glEnableVertexAttribArray(index);
    glBindVertexArray(0);
}

void BasicShapeArrays::draw(GLenum mode, GLsizei count) {
    // Partie 1: Dessiner la forme.
    glBindVertexArray(this->m_vao);
    glDrawArrays(mode, 0, count);
    glBindVertexArray(0);
}


BasicShapeElements::BasicShapeElements(const GLfloat *data, GLsizeiptr byteSize, const GLuint *indexes,
                                       GLsizeiptr indexesByteSize) {
    // Partie 1: Générer et bind le vao de la forme.
    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);

    // Partie 1: Générer et bind le vbo et ebo de la forme.
    // Allouer l'espace nécessaire dans le mode approprié
    // et envoyer les données au gpu.
    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);

    glGenBuffers(1, &this->m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesByteSize, indexes, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

BasicShapeElements::BasicShapeElements() {
    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);
    glGenBuffers(1, &this->m_vbo);
    glGenBuffers(1, &this->m_ebo);
    glBindVertexArray(0);
}

BasicShapeElements::~BasicShapeElements() {
    // Partie 1: Supprimer la mémoire de l'objet.
    // Assurez-vous que les ressources ne soient pas utilisées
    // pendant la suppression.
    glDeleteBuffers(1, &this->m_vbo);
    glDeleteBuffers(1, &this->m_ebo);
    glDeleteVertexArrays(1, &this->m_vao);
}

void BasicShapeElements::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
    // Partie 1: Activer un attribut et l'attacher correctement au state du vao.
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (GLvoid *) offset);
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void BasicShapeElements::setData(const GLfloat *data, GLsizeiptr byteSize, const GLuint *indexes,
                                 GLsizeiptr indexesByteSize) {
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesByteSize, indexes, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void BasicShapeElements::draw(GLenum mode, GLsizei count) {
    // Partie 1: Dessiner la forme avec le ebo.
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
