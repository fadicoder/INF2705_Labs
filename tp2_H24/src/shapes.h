#ifndef SHAPES_H
#define SHAPES_H

#include <GL/glew.h>

class BasicShapeArrays {
public:
    BasicShapeArrays(const GLfloat *data, GLsizeiptr byteSize);

    ~BasicShapeArrays();

    void enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset);

    void draw(GLenum mode, GLsizei count);

private:
    GLuint m_vao;
    GLuint m_vbo;
};

class BasicShapeElements {
public:
    BasicShapeElements(const GLfloat *data, GLsizeiptr byteSize, const GLuint *indexes, GLsizeiptr indexesByteSize);

    BasicShapeElements();

    ~BasicShapeElements();

    void enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset);

    void setData(const GLfloat *data, GLsizeiptr byteSize, const GLuint *indexes, GLsizeiptr indexesByteSize);

    void draw(GLenum mode, GLsizei count);

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
};

#endif // SHAPES_H
