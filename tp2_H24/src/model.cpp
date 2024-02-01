#include "model.h"

#include "obj_loader.h"
#include "shapes.h"
#include <iostream>


Model::Model(const char *path) {
    // TODO: Initalisation du modèle et des attibuts de la classe
    std::vector<GLfloat> data;
    std::vector<GLuint> indices;
    this->loadObj(path, data, indices);
    this->m_count = (GLsizei) indices.size();
    this->m_shape = BasicShapeElements((GLfloat*) &data,
                                       (GLsizei) (data.size() * sizeof(GLfloat)),
                                       (GLuint*) &indices,
                                       (GLsizei) (indices.size() * sizeof(GLuint)));
}

void Model::loadObj(const char *path, std::vector<GLfloat> &vertexData, std::vector<GLuint> &indices) {
    objl::Loader loader;
    bool loadout = loader.LoadFile(path);
    if (!loadout) {
        std::cout << "Unable to load model " << path << std::endl;
        return;
    }

    for (auto & loadedVertex: loader.LoadedVertices) {
        objl::Vector3 p = loadedVertex.Position;
        vertexData.push_back(p.X);
        vertexData.push_back(p.Y);
        vertexData.push_back(p.Z);
        // TODO: Décommenter lors de la partie 2
        //objl::Vector2 t = loader.LoadedVertices[i].TextureCoordinate;
        //vertexData.push_back(t.X);
        //vertexData.push_back(t.Y);
    }
    indices = loader.LoadedIndices;
}

void Model::draw() {
    this->m_shape.draw(GL_TRIANGLES, this->m_count);
    // TODO: Dessine le modèle en triangle
}
