#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    m_shape = NULL;
}

SceneviewScene::~SceneviewScene()
{
    // TODO: [SCENEVIEW] Don't leak memory!
}


void SceneviewScene::init()
{
    OpenGLScene::init();

    // Initialize the vertex array object.
    decideParameter();

    std::vector<primitiveNmatrix>::iterator it;
    for(it = m_tbd.begin(); it != m_tbd.end(); it++)
    {
        primitiveNmatrix *m = &(*it);
        m_styp = m->shapetype;

        std::map<std::string, GLuint>::iterator itm;
        instantiateShape();
        m->m_vaoID = m_shape->m_vaoID;
        if(m->material.textureMap->isUsed)
        {
            itm = m_texMap.find(m->material.textureMap->filename);
            if (itm != m_texMap.end())
            {
                m_texMap[m->material.textureMap->filename] = loadTexture(m->material.textureMap->filename);
                m->material.textureMap->texid = m_texMap[m->material.textureMap->filename];
            }
            else
                m->material.textureMap->texid = m_texMap[m->material.textureMap->filename];
        }
        m->verticesNumber = m_shape->getVerticesNumber();
    }
}

void SceneviewScene::setLights(const glm::mat4 viewMatrix)
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Use function(s) inherited from OpenGLScene to set up the lighting for your scene.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //
    for (int id = 0; id < MAX_NUM_LIGHTS; ++id)
        glDisable(GL_LIGHT0 + id);

    std::vector<CS123SceneLightData>::iterator it;
    for(it = m_scenelights.begin(); it !=  m_scenelights.end(); it++)
        setLight((*it));
}

void SceneviewScene::renderGeometry()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.

    std::vector<primitiveNmatrix>::iterator it;
    for(it = m_tbd.begin(); it != m_tbd.end(); it++)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "m"), 1, GL_FALSE, &(((*it).comMatrix)[0][0]));

        CS123SceneMaterial material = (*it).material;

        material.cAmbient = (*it).material.cAmbient * m_sceneglobalData.ka;
        material.cDiffuse = (*it).material.cDiffuse * m_sceneglobalData.kd;
        material.cSpecular = (*it).material.cSpecular * m_sceneglobalData.ks;
        material.cTransparent = (*it).material.cTransparent * m_sceneglobalData.kt;

        applyMaterial(material);

        glBindVertexArray((*it).m_vaoID);
        glDrawArrays(GL_TRIANGLES, 0, (*it).verticesNumber/* Number of vertices to draw */);
        glBindVertexArray(0);

    }

}

void SceneviewScene::setSelection(int x, int y)
{
    // TODO: [MODELER LAB] Fill this in...
    //
    // Using m_selectionRecorder, set m_selectionIndex to the index in your
    // flattened parse tree of the object under the mouse pointer.  The
    // selection recorder will pick the object under the mouse pointer with
    // some help from you, all you have to do is:

    // 1) Set this to the number of objects you will be drawing.
    int numObjects = 0;

    // 2) Start the selection process
    m_selectionRecorder.enterSelectionMode(x, y, numObjects);

    // 3) Draw your objects, calling m_selectionRecorder.setObjectIndex() before drawing each one.

    // 4) Find out which object you selected, if any (-1 means no selection).
    m_selectionIndex = m_selectionRecorder.exitSelectionMode();
}

void SceneviewScene::decideParameter()
{
    uint nodenum = m_tbd.size();
    if(nodenum < 100)
    {
        m_sp1 = 12 + 1*((100-nodenum)/5);
        m_sp2 = 17 + 1*((100-nodenum)/5);
    }
    else if(nodenum <= 400 && nodenum >= 100)
    {
        m_sp1 = 14 - 1*(nodenum/50);
        m_sp2 = 19 - 1*(nodenum/50);
    }
    else
    {
        m_sp1 = 6;
        m_sp2 = 11;
    }

}

GLuint SceneviewScene::loadTexture(const std::string &filename)
{
    // Make sure the image file exists
    QFile file(QString::fromStdString(filename));
    if (!file.exists())
        return -1;

    // Load the file into memory
    QImage image;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    QImage texture = QGLWidget::convertToGLFormat(image);

    // Generate a new OpenGL texture ID to put our image into
    GLuint id = 0;
    glGenTextures(1, &id);

    // Make the texture we just created the new active texture
    glBindTexture(GL_TEXTURE_2D, id);

    // Set coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,texture.width(), texture.height(),0, GL_RGBA,GL_UNSIGNED_BYTE, texture.bits());

    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}
