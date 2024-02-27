#include "model.h"

#include "obj_loader.h"
#include "shapes.h"
#include <iostream>

Model::Model() : m_count(0){}

Model::Model(const char *path) {
    std::vector<GLfloat> data;
    std::vector<GLuint> indices;
    this->loadObj(path, data, indices);
    this->m_count = (GLsizei) indices.size();
    this->m_shape.setData(&data[0],
                                       (GLsizei) (data.size() * sizeof(GLfloat)),
                                       &indices[0],
                                       (GLsizei) (indices.size() * sizeof(GLuint)));
     this->m_shape.enableAttribute(0, 3, sizeof(float) * 5, 0);
     this->m_shape.enableAttribute(1, 2, sizeof(float) * 5, sizeof(float) * 3);
}

void Model::loadObj(const char *path, std::vector<GLfloat> &vertexData, std::vector<GLuint> &indices) {
    objl::Loader loader;
    bool loadout = loader.LoadFile(path);
    if (!loadout) {
        std::cout << "Unable to load model " << path << std::endl;
        return;
    }
    for (auto &loadedVertex: loader.LoadedVertices) {
        objl::Vector3 p = loadedVertex.Position;
        vertexData.push_back(p.X);
        vertexData.push_back(p.Y);
        vertexData.push_back(p.Z);
        // Décommenter lors de la partie 2
        objl::Vector2 t = loadedVertex.TextureCoordinate;
        vertexData.push_back(t.X);
        vertexData.push_back(t.Y);
    }
    indices = loader.LoadedIndices;
}

void Model::draw() {
    this->m_shape.draw(GL_TRIANGLES, this->m_count);
}
