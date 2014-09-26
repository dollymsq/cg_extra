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
    m_shape = NULL;
    m_sp1 = 0;
    m_sp2 = 0;
    m_sp3 = 0;
    m_styp = -1;
}

ShapesScene::~ShapesScene()
{
    // TODO: [SHAPES] Don't leak memory!
    if(m_shape!=NULL)
        delete m_shape;

    glDeleteVertexArrays(m_vsize, &m_vaoID);
    glDeleteBuffers(m_vsize, &vertexBuffer);
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

    // Initialize the vertex array object.
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    // Initialize the vertex buffer object.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Remember for large arrays you should use new
    instantiateShape();

    // Don't forget to clean up any resources you created
}

void ShapesScene::renderGeometry()
{
    // TODO: [SHAPES] Render the shape. The sample code provided draws the 3 vertices defined in
    //       init() above.

    applyMaterial(m_material);

    // Draw the shape.
    glBindVertexArray(m_vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    instantiateShape();
    if(m_shape != NULL)
    {
        m_vsize = m_shape->getVerticesNumber();
        glBindVertexArray(m_vaoID);
        glDrawArrays(GL_TRIANGLES, 0, m_vsize/* Number of vertices to draw */);
        glBindVertexArray(0);
    }
}

void ShapesScene::setLights(const glm::mat4 viewMatrix)
{
    // YOU DON'T NEED TO TOUCH THIS METHOD, unless you want to do fancy lighting...

    m_light.dir = glm::inverse(viewMatrix) * lightDirection;

    clearLights();
    setLight(m_light);
}

void ShapesScene::instantiateShape()
{
    if((m_sp1!=settings.shapeParameter1)||(m_sp2!=settings.shapeParameter2)||(m_sp3!=settings.shapeParameter3)||(m_styp!=settings.shapeType))
    {
        m_sp1 = settings.shapeParameter1;
        m_sp2 = settings.shapeParameter2;
        m_sp3 = settings.shapeParameter3;
        m_styp = settings.shapeType;
        if(m_shape!= NULL)
        {
            delete m_shape;
            m_shape = NULL;
        }
        switch(m_styp)
        {
            case SHAPE_CUBE:
                m_shape = new Cube(m_sp1, m_sp2, m_sp3);
                break;
            case SHAPE_CONE:
                m_shape = new Cone(m_sp1, m_sp2, m_sp3);
                break;
            case SHAPE_CYLINDER:
                m_shape = new Cylinder(m_sp1, m_sp2, m_sp3);
                break;
            case SHAPE_SPHERE:
                m_shape = new Sphere(m_sp1, m_sp2, m_sp3);
                break;
            case SHAPE_TORUS:
                m_shape = new Torus(m_sp1, m_sp2, m_sp3);
                break;
            case SHAPE_FRACTAL:
                m_shape = new Fractal(m_sp1, m_sp2, m_sp3);
                break;
            default:
                m_normalRenderer->clearArrays();
                break;
        }
        if(m_shape != NULL)
            m_shape->packVerticesintoBuffer(m_shader, m_normalRenderer);
    }
}
