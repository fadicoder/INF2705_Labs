#ifndef VERTICES_DATA_H
#define VERTICES_DATA_H

const GLfloat colorSquareVerticesReduced[] = {
        -0.15f, -0.15f, 0.0f, 0.0f, 0.0f,
        0.15f, -0.15f, 0.0f, 1.0f, 0.0f,
        -0.15f, 0.15f, 0.0f, 0.0f, 1.0f,
        0.15f, 0.15f, 0.0f, 1.0f, 1.0f,
};

const GLuint indexes[] = {
        0, 1, 2,
        1, 3, 2
};

const GLfloat groundVertices[] = {
        -30.0f, -1.0f, -30.0f, 0.0f, 0.0f,
        30.0f, -1.0f, -30.0f, 10.0f, 0.0f,
        -30.0f, -1.0f, 30.0f, 0.0f, 10.0f,
        30.0f, -1.0f, 30.0f, 10.0f, 10.0f,
};

const GLuint groundIndexes[] = {
        0, 1, 2,
        1, 3, 2,
};


const GLfloat riverVertices[] = {
        30.0f, -1.0f, -30.0f, 0.0f, 0.0f,
        50.0f, -1.0f, -30.0f, 3.0f, 0.0f,
        30.0f, -1.0f, 30.0f, 0.0f, 10.0f,
        50.0f, -1.0f, 30.0f, 3.0f, 10.0f,
};

const GLuint riverIndexes[] = {
        0, 1, 2,
        1, 3, 2,
};


const GLfloat skyboxVertices[] = {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
};

#endif // VERTICES_DATA_H
