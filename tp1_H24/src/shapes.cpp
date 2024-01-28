#include "shapes.h"

BasicShapeArrays::BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize)
{
    // Partie 1: Générer et bind le vao de la forme.
    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);

    // Partie 1: Générer et bind le vbo de la forme.
    // Allouer l'espace nécessaire dans le mode approprié
    // et envoyer les données au gpu.
    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    
    glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
}

BasicShapeArrays::~BasicShapeArrays()
{
    // Partie 1: Supprimer la mémoire de l'objet.
    // Assurez-vous que les ressources ne soient pas utilisées
    // pendant la suppression.
    glDeleteBuffers(1, &this->m_vbo);
    glDeleteVertexArrays(1, &this->m_vao);
}

void BasicShapeArrays::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // Partie 1: Activer un attribut et l'attacher correctement au state du vao.
    glBindVertexArray(this->m_vao);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride,  (GLvoid*)offset);
    glEnableVertexAttribArray(index);
    glBindVertexArray(0);
}

void BasicShapeArrays::draw(GLenum mode, GLsizei count)
{
    // Partie 1: Dessiner la forme.
    glBindVertexArray(this->m_vao);
    glDrawArrays(mode, 0, count);
    glBindVertexArray(0);
}



BasicShapeMultipleArrays::BasicShapeMultipleArrays(const GLfloat* pos, GLsizeiptr posByteSize, const GLfloat* color, GLsizeiptr colorByteSize)
{
    // Partie 1: Générer et bind le vao de la forme.
    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);
    
    // Partie 1: Générer et bind les vbos (un pour la position,
    // l'autre pour la couleur) de la forme. Allouer l'espace
    // nécessaire dans le mode approprié et envoyer les données au gpu.
    glGenBuffers(1, &this->m_posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_posVbo);
    
    glBufferData(GL_ARRAY_BUFFER, posByteSize, pos, GL_STATIC_DRAW);

    // Couleur
    glGenBuffers(1, &this->m_colorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_colorVbo);
    
    glBufferData(GL_ARRAY_BUFFER, colorByteSize, color, GL_STATIC_DRAW);
}

BasicShapeMultipleArrays::~BasicShapeMultipleArrays()
{
    // Partie 1: Supprimer la mémoire de l'objet.
    // Assurez-vous que les ressources ne soient pas utilisées
    // pendant la suppression.
    glDeleteBuffers(1, &this->m_posVbo);
    glDeleteBuffers(1, &this->m_colorVbo);
    glDeleteVertexArrays(1, &this->m_vao);
}

void BasicShapeMultipleArrays::enablePosAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // Partie 1: Activer l'attribut de position et l'attacher correctement au state du vao.
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_posVbo);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride,  (GLvoid*)offset);
    glEnableVertexAttribArray(index);
    glBindVertexArray(0);
}

void BasicShapeMultipleArrays::enableColorAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // Partie 1: Activer l'attribut de couleur et l'attacher correctement au state du vao.
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_colorVbo);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride,  &offset);
    glEnableVertexAttribArray(index);
    glBindVertexArray(0);
}

void BasicShapeMultipleArrays::updateColorData(const GLfloat* color, GLsizeiptr colorByteSize)
{
    // Partie 1: Modifier la totalité des données de couleur
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_colorVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colorByteSize, color);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLfloat* BasicShapeMultipleArrays::mapPosData()
{
    // Partie 1: Activer le mapping des données de position
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_posVbo);
    void* address = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return (GLfloat*) address;

}

void BasicShapeMultipleArrays::unmapPosData()
{
    // Partie 1: Désactiver le mapping des données de position
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_posVbo);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void BasicShapeMultipleArrays::draw(GLenum mode, GLsizei count)
{
    // Partie 1: Dessiner la forme.
    glBindVertexArray(this->m_vao);
    glDrawArrays(mode, 0, count);
    glBindVertexArray(0);
}



BasicShapeElements::BasicShapeElements(const GLfloat* data, GLsizeiptr byteSize, const GLubyte* indexes, GLsizeiptr indexesByteSize)
{
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
}

BasicShapeElements::~BasicShapeElements()
{
    // Partie 1: Supprimer la mémoire de l'objet.
    // Assurez-vous que les ressources ne soient pas utilisées
    // pendant la suppression.
    glDeleteBuffers(1, &this->m_vbo);
    glDeleteBuffers(1, &this->m_ebo);
    glDeleteVertexArrays(1, &this->m_vao);
}

void BasicShapeElements::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // Partie 1: Activer un attribut et l'attacher correctement au state du vao.
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vao);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride,  (GLvoid*)offset);
    glEnableVertexAttribArray(index);
}

void BasicShapeElements::draw(GLenum mode, GLsizei count)
{
    // Partie 1: Dessiner la forme avec le ebo.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glDrawArrays(mode, 0, count);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
