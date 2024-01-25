#include "shader_program.h"

#include <iostream>

Shader::Shader(GLenum type, const char* code) {
    // Partie 1: Créer et compiler le code du shader.
    // Il est toujours bonne pratique de vérifier s'il y a
    // des erreurs à la fin.
    this->m_id = glCreateShader(type);
    glShaderSource(this->m_id, 1, &code, NULL);
    glCompileShader(this->m_id);
    this->checkError();
}
    
Shader::~Shader() {
    // Partie 1: Suppression du shader.
    glDeleteShader(this->m_id);
}

GLuint Shader::id() {
    // Partie 1: Retourner l'id du shader.
    return this->m_id;
}

void Shader::checkError() {
    GLint success;
    GLchar infoLog[1024];

    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_id, 1024, NULL, infoLog);
        glDeleteShader(m_id);
        std::cout << "Shader compile error: " << infoLog << std::endl;
    }
}



ShaderProgram::ShaderProgram() {
    // Partie 1: Créer un programme d'OpenGL.
    this->m_id = glCreateProgram();
    
}
    
ShaderProgram::~ShaderProgram(){
    // Partie 1: Suppression du programme d'OpenGL.
    glDeleteProgram(this->m_id);
}
    
void ShaderProgram::use()
{
    // Partie 1: Utiliser le programme pour le rendu.
    glUseProgram(this->m_id);
}
    
void ShaderProgram::attachShader(Shader& s)
{        
    // Partie 1: Attacher un nouveau shader au programme.
    glAttachShader(this->m_id, s.id());
}
    
void ShaderProgram::link()
{        
    // Partie 1: Faire le linking du programme.
    // N'oublier pas de tester pour les erreurs.
    glLinkProgram(this->m_id);
    this->checkError();
}

GLint ShaderProgram::getUniformLoc(const char* name)
{
    // Partie 2: Retourne la location d'un uniform dans le programme
    return glGetUniformLocation(this->m_id, name);
}
    
void ShaderProgram::checkError()
{
    GLint success;
    GLchar infoLog[1024];

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, 1024, NULL, infoLog);
        glDeleteProgram(m_id);
        std::cout << "Program linking error: " << infoLog << std::endl;
    }
}
