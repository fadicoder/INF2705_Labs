#include "resources.h"

#include "vertices_data.h"
#include "utils.h"

const char* pathes[] = {
    "../textures/skybox/Daylight Box_Right.bmp",
    "../textures/skybox/Daylight Box_Left.bmp",
    "../textures/skybox/Daylight Box_Top.bmp",
    "../textures/skybox/Daylight Box_Bottom.bmp",
    "../textures/skybox/Daylight Box_Front.bmp",
    "../textures/skybox/Daylight Box_Back.bmp",
};

Resources::Resources()
: ground(groundVertices, sizeof(groundVertices), groundIndexes, sizeof(groundIndexes))
, skybox(skyboxVertices, sizeof(skyboxVertices))
, rock("../models/rock.obj")
, suzanne("../models/suzanne.obj")
, glass("../models/glass.obj")
, sphere("../models/icosphere.obj")
, cube("../models/cube.obj")
, spotlight("../models/spotlight.obj")
, rockTexture("../models/rockTexture.png", GL_CLAMP_TO_EDGE)
, suzanneTexture("../models/suzanneTexture.png", GL_CLAMP_TO_EDGE)
, glassTexture("../models/glass.png", GL_CLAMP_TO_EDGE)
, groundTexture("../textures/groundSeamless.jpg", GL_REPEAT)
, whiteTexture("../textures/white.png", GL_CLAMP_TO_EDGE)
, diffuseMapTexture("../textures/metal_0029_color_1k.jpg", GL_CLAMP_TO_EDGE)
, specularMapTexture("../textures/metal_0029_metallic_1k.jpg", GL_CLAMP_TO_EDGE)
, skyboxTexture(pathes)
{
    // Simple meshes
    ground.enableAttribute(0, 3, 5, 0);
    ground.enableAttribute(1, 2, 5, 3);

    skybox.enableAttribute(0, 3, 0, 0);

    // Textures
    groundTexture.enableMipmap();

    // Model shader
    {
        model.setupShaderProgram("shaders/model.fs.glsl", "shaders/model.vs.glsl");
        mvpLocationModel = model.getUniformLoc("mvp");
    }
    GL_CHECK_ERROR;

    // Phong shader
    {
        phong.setupShaderProgram("shaders/phong.fs.glsl", "shaders/phong.vs.glsl");
        mvpLocationPhong = phong.getUniformLoc("mvp");
        modelViewLocationPhong = phong.getUniformLoc("modelView");
        viewLocationPhong = phong.getUniformLoc("view");
        normalLocationPhong = phong.getUniformLoc("normalMatrix");
        phong.use();
        glUniform1i(phong.getUniformLoc("diffuseSampler"), 0);
        glUniform1i(phong.getUniformLoc("specularSampler"), 1);
    }
    GL_CHECK_ERROR;

    // Gouraud shader
    {
        gouraud.setupShaderProgram("shaders/gouraud.fs.glsl", "shaders/gouraud.vs.glsl");
        mvpLocationGouraud = gouraud.getUniformLoc("mvp");
        modelViewLocationGouraud = gouraud.getUniformLoc("modelView");
        viewLocationGouraud = gouraud.getUniformLoc("view");
        normalLocationGouraud = gouraud.getUniformLoc("normalMatrix");
        gouraud.use();
        glUniform1i(gouraud.getUniformLoc("diffuseSampler"), 0);
        glUniform1i(gouraud.getUniformLoc("specularSampler"), 1);
    }
    GL_CHECK_ERROR;

    // Flat shader
    {
        flat.setupShaderProgram("shaders/gouraud.fs.glsl", "shaders/flat.vs.glsl", "shaders/flat.gs.glsl");
        mvpLocationFlat = flat.getUniformLoc("mvp");
        modelViewLocationFlat = flat.getUniformLoc("modelView");
        viewLocationFlat = flat.getUniformLoc("view");
        normalLocationFlat = flat.getUniformLoc("normalMatrix");
        flat.use();
        glUniform1i(flat.getUniformLoc("diffuseSampler"), 0);
        glUniform1i(flat.getUniformLoc("specularSampler"), 1);
    }
    GL_CHECK_ERROR;


    // Simple shader
    {
        simple.setupShaderProgram("shaders/simple.fs.glsl", "shaders/simple.vs.glsl");
        mvpLocationSimple = simple.getUniformLoc("mvp");
        colorLocationSimple = simple.getUniformLoc("color");
    }
    GL_CHECK_ERROR;

    // Skybox shader
    {
        skyboxShader.setupShaderProgram("shaders/skybox.fs.glsl", "shaders/skybox.vs.glsl");
        mvpLocationSky = skyboxShader.getUniformLoc("mvp");
    }
    GL_CHECK_ERROR;
}
