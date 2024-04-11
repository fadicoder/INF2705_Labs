#include "scenes.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"

#include "utils.h"

Scene::Scene(Resources& resources)
: m_res(resources)
{

}

Scene::~Scene() = default;

void Scene::drawSky(glm::mat4& mvp)
{
    // DRAW SKYBOX
    glDepthFunc(GL_LEQUAL);
    m_res.skyboxShader.use();
    m_res.skyboxTexture.use();
    glUniformMatrix4fv(m_res.mvpLocationSky, 1, GL_FALSE, &mvp[0][0]);
    m_res.skybox.draw(GL_TRIANGLES, 6 * 6);
    glDepthFunc(GL_LESS);
}

TesselationScene::TesselationScene(Resources& resources)
: Scene(resources)
, m_viewWireframe(false)
{
    glPatchParameteri(GL_PATCH_VERTICES, 4);
}

TesselationScene::~TesselationScene() = default;

void TesselationScene::render(glm::mat4& view, glm::mat4& projPersp)
{
    drawMenu();

    glm::mat4 mvp;
    glm::mat4 projView = projPersp * view;
    glm::mat4 modelView;

    m_res.tessellation.use();
    m_res.heightmapTexture.use(0);
    m_res.grassTexture.use(1);
    m_res.sandTexture.use(2);
    m_res.snowTexture.use(3);

    mvp = projView;
    glUniformMatrix4fv(m_res.mvpLocationTessellation, 1, GL_FALSE, &mvp[0][0]);
    modelView = view;
    glUniformMatrix4fv(m_res.modelViewLocationTessellation, 1, GL_FALSE, &modelView[0][0]);

    glUniform1i(m_res.viewWireframeLocationTessellation, m_viewWireframe);

    m_res.tesselationPlane.draw(GL_PATCHES, m_res.tesselationPlaneCount);

    mvp = projPersp * glm::mat4(glm::mat3(view));
    drawSky(mvp);
}

void TesselationScene::drawMenu()
{
    ImGui::Begin("Tessellation Parameters");

    ImGui::Checkbox("View wireframe?", (bool*)&m_viewWireframe);

    ImGui::End();
}


static const unsigned int MAX_N_PARTICULES = 10000;
static Particle particles[MAX_N_PARTICULES] = { {{0,0,0},{0,0,0},{0,0,0,0}, {0,0},0} };

ParticleScene::ParticleScene(Resources& resources, Window& w)
: Scene(resources)
, m_w(w)
, m_oldTime(m_w.getTick() / 1000.0f)
, m_cumulativeTime(0.0f)
, m_nParticles(1)
, m_nMaxParticles(1000)
{
    glEnable(GL_PROGRAM_POINT_SIZE);
    GL_CHECK_ERROR;

    glGenVertexArrays(1, &this->m_vao);
    GL_CHECK_ERROR;
    glBindVertexArray(this->m_vao);
    GL_CHECK_ERROR;
    glGenBuffers(2, this->m_vbo);
    GL_CHECK_ERROR;
    glGenQueries(1, &m_nParticles);
    glGenTransformFeedbacks(1, &m_tfo);
    GL_CHECK_ERROR;
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, MAX_N_PARTICULES * sizeof(Particle), particles, GL_DYNAMIC_COPY);
    GL_CHECK_ERROR;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(float),  (GLvoid*) nullptr);
    glEnableVertexAttribArray(0);
    GL_CHECK_ERROR;

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(float),  (GLvoid*) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    GL_CHECK_ERROR;

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 13 * sizeof(float),  (GLvoid*) (sizeof(float) * 6));
    glEnableVertexAttribArray(2);
    GL_CHECK_ERROR;

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 13 * sizeof(float),  (GLvoid*) (sizeof(float) * 10));
    glEnableVertexAttribArray(3);
    GL_CHECK_ERROR;

    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 13 * sizeof(float),  (GLvoid*) (sizeof(float) * 12));
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, MAX_N_PARTICULES * sizeof(Particle), nullptr, GL_DYNAMIC_COPY);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->m_tfo);
    GL_CHECK_ERROR;
}

ParticleScene::~ParticleScene()
{
    glDeleteBuffers(2, this->m_vbo);
    glDeleteVertexArrays(1, &this->m_vao);
    glDisable(GL_PROGRAM_POINT_SIZE);
}

void ParticleScene::render(glm::mat4& view, glm::mat4& projPersp)
{
    glm::mat4 mvp;
    glm::mat4 projView = projPersp * view;
    glm::mat4 modelView;

    float time = m_w.getTick() / 1000.0;
    float dt = time - m_oldTime;
    m_oldTime = time;
    m_cumulativeTime += dt;
    if (dt > 1.0f)
        m_nParticles = 1;

    m_res.transformFeedback.use();
    
    // buffer binding
    glBindVertexArray(m_vao);

    glUniform1f(m_res.timeLocationTransformFeedback, time);
    glUniform1f(m_res.dtLocationTransformFeedback, dt);

    // update particles
    glDrawBuffer(GL_POINTS);
    GL_CHECK_ERROR;

    // swap buffers
    this->m_w.swap();

    // Draw skybox first without the function to change some parameter on the depth test.
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    m_res.skyboxShader.use();
    m_res.skyboxTexture.use();
    mvp = projPersp * glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(m_res.mvpLocationSky, 1, GL_FALSE, &mvp[0][0]);
    m_res.skybox.draw(GL_TRIANGLES, 6 * 6);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);


    m_res.particule.use();

    m_res.flameTexture.use();

    // TODO: buffer binding
    glBindVertexArray(this->m_vao);

    modelView = view;
    glUniformMatrix4fv(m_res.modelViewLocationParticle, 1, GL_FALSE, &modelView[0][0]);
    glUniformMatrix4fv(m_res.projectionLocationParticle, 1, GL_FALSE, &projPersp[0][0]);

    // TODO: Draw particles without depth write and with blending
    m_res.particule.use();
    m_res.flameTexture.use();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawBuffer(GL_STREAM_DRAW);
    GL_CHECK_ERROR;

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    GL_CHECK_ERROR;

    if (m_cumulativeTime > 1.0f / 60.0f)
    {
        m_cumulativeTime = 0.0f;
        if (++m_nParticles > m_nMaxParticles)
            m_nParticles = m_nMaxParticles;
    }    
}
