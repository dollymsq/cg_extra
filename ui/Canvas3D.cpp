#include "Canvas3D.h"

#include "Camera.h"
#include "OpenGLScene.h"
#include "OrbitingCamera.h"
#include "ResourceLoader.h"
#include "Settings.h"

Canvas3D::Canvas3D(QGLFormat format, QWidget *parent) : SupportCanvas3D(format, parent)
{
}

Canvas3D::~Canvas3D()
{
}

void Canvas3D::initializeGL()
{
    // Track the camera settings so we can generate deltas
    m_oldPosX = settings.cameraPosX;
    m_oldPosY = settings.cameraPosY;
    m_oldPosZ = settings.cameraPosZ;
    m_oldRotU = settings.cameraRotU;
    m_oldRotV = settings.cameraRotV;
    m_oldRotN = settings.cameraRotN;

    //initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    glGetError(); // Clear errors after call to glewInit
    if (GLEW_OK != err)
    {
      // Problem: glewInit failed, something is seriously wrong.
      fprintf(stderr, "Error initializing glew: %s\n", glewGetErrorString(err));
    }

    // Initialize scene
    OpenGLScene *glScene = this->getScene();
    if (glScene != NULL)
        glScene->init();

    // Enable depth testing, so that objects are occluded based on depth instead of drawing order.
    glEnable(GL_DEPTH_TEST);

    // Move the polygons back a bit so lines are still drawn even though they are coplanar with the
    // polygons they came from, which will be drawn before them.
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1, -1);

    // Enable back-face culling, meaning only the front side of every face is rendered.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Specify that the front face is represented by vertices in counterclockwise order (this is
    // the default).
    glFrontFace(GL_CCW);

    // Calculate the orbiting camera matrices.
    getOrbitingCamera()->updateMatrices();

    m_initialized = true;
}


void Canvas3D::paintGL()
{
    OpenGLScene *glScene = this->getScene();

    if (glScene != NULL)
    {
        // Check for errors from the last frame.
        int err;
        if ((err = glGetError()) != GL_NO_ERROR) {
            fprintf(stderr, "GL is in an error state before painting.\n");
            fprintf(stderr, "(GL error code %d)\n", err);
        }

        // Update the scene camera.
        glViewport(0, 0, width(), height());
        getCamera()->setAspectRatio((float)width() / (float)height());

        // Render the scene.
        glScene->render(this);
    }
    else
    {
        // Display a placeholder message explaining that no OpenGLScene is currently loaded.
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO figure out a way to display "No OpenGLScene loaded"
    }
}


void Canvas3D::settingsChanged()
{
    // TODO: Process changes to the application settings.


    // Call superclass (this repaints the scene for you)
    SupportCanvas3D::settingsChanged();
}
