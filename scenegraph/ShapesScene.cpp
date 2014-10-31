#include "ShapesScene.h"
#include "Camera.h"
#include <SupportCanvas3D.h>
#include <QFileDialog>


glm::vec4 lightDirection = glm::normalize(glm::vec4(1.f, -1.f, -1.f, 0.f));

ShapesScene::ShapesScene()
{
    // Use a shiny orange material
    memset(&m_material, 0, sizeof(m_material));
    m_material.cAmbient.r = 0.2f;
    m_material.cAmbient.g = 0.1f;
    m_material.cDiffuse.r = 1;
    m_material.cDiffuse.g = 0.5f;
    m_material.cSpecular.r = m_material.cSpecular.g = m_material.cSpecular.b = 1;
    m_material.shininess = 64;

    // Use a white directional light from the upper left corner
    memset(&m_light, 0, sizeof(m_light));
    m_light.type = LIGHT_DIRECTIONAL;
    m_light.dir = lightDirection;
    m_light.color.r = m_light.color.g = m_light.color.b = 1;
    m_light.id = 0;

    //TODO: [SHAPES] Allocate any additional memory you need...
}

ShapesScene::~ShapesScene()
{
    // TODO: [SHAPES] Don't leak memory!
}

void ShapesScene::init()
{
    // TODO: [SHAPES] Initialize the shape based on settings.shapeType. The sample code provided
    //       initializes a single triangle using OpenGL. Refer to the labs for more information
    //       about these OpenGL functions.
    //
    //       If you've chosen to implement subclasses for your shapes, this would be a good time to
    //       call upon the subclass to initialize itself.
    //

    OpenGLScene::init(); // Call the superclass's init()

    // Remember for large arrays you should use new

}

void ShapesScene::renderGeometry()
{
    // TODO: [SHAPES] Render the shape. The sample code provided draws the 3 vertices defined in
    //       init() above.


    // Draw the shape.

    if((m_sp1!=settings.shapeParameter1)||(m_sp2!=settings.shapeParameter2)||(m_sp3!=settings.shapeParameter3)||(m_styp!=settings.shapeType))
    {
        m_sp1 = settings.shapeParameter1;
        m_sp2 = settings.shapeParameter2;
        m_sp3 = settings.shapeParameter3;
        m_styp = settings.shapeType;
        instantiateShape(m_material);
    }
    applyMaterial(m_material);

//    if(m_shape != NULL)
//        m_shapevaoDraw();
    glBindVertexArray(m_shape->m_vaoID);
    m_vsize = m_shape->getVerticesNumber();
    glDrawArrays(GL_TRIANGLES, 0, m_vsize/* Number of vertices to draw */);
    glBindVertexArray(0);

}

void ShapesScene::setLights(const glm::mat4 viewMatrix)
{
    // YOU DON'T NEED TO TOUCH THIS METHOD, unless you want to do fancy lighting...

    m_light.dir = glm::inverse(viewMatrix) * lightDirection;

    clearLights();
    setLight(m_light);
}


