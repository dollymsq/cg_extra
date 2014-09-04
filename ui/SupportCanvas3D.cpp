#include "SupportCanvas3D.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include "ShapesScene.h"
#include "SceneviewScene.h"
#include "OrbitingCamera.h"
#include "CamtransCamera.h"
#include "Settings.h"

SupportCanvas3D::SupportCanvas3D(QGLFormat format, QWidget *parent) : QGLWidget(format, parent)
{
    m_initialized = false;
    m_isDragging = false;
    m_scene = NULL;

    m_defaultPerspectiveCamera = new CamtransCamera;
    m_defaultOrbitingCamera = new OrbitingCamera;

    settingsChanged();
}

SupportCanvas3D::~SupportCanvas3D()
{
    delete m_defaultPerspectiveCamera;
    delete m_defaultOrbitingCamera;
    delete m_scene;
}

Camera *SupportCanvas3D::getCamera()
{
    switch (settings.getCameraMode())
    {
    case CAMERAMODE_CAMTRANS:
        return m_defaultPerspectiveCamera;

    case CAMERAMODE_ORBIT:
        return m_defaultOrbitingCamera;

    default:
        return NULL;
    }
}

OrbitingCamera *SupportCanvas3D::getOrbitingCamera()
{
    return m_defaultOrbitingCamera;
}


CamtransCamera *SupportCanvas3D::getCamtransCamera()
{
    return m_defaultPerspectiveCamera;
}

void SupportCanvas3D::settingsChanged()
{
    ShapesScene *sh;
    SceneviewScene *sv;

    switch (settings.getSceneMode())
    {

    case SCENEMODE_SHAPES:
        // Make sure we are showing a ShapesScene.
        sh = dynamic_cast<ShapesScene *>(this->getScene());
        if (!sh)
        {
            sh = new ShapesScene;
            if (isInitialized()) sh->init();
            setScene(sh);
        }
        break;

    case SCENEMODE_SCENEVIEW:
        // Make sure we are showing a SceneviewScene.
        sv = dynamic_cast<SceneviewScene *>(this->getScene());
        if (!sv) setScene(NULL);
        break;
    }

    update(); /* repaint the scene */
}

bool SupportCanvas3D::isInitialized()
{
    return m_initialized;
}

void SupportCanvas3D::setScene(OpenGLScene *scene)
{
    delete m_scene;
    m_scene = scene;

    // If initializeGL was already called, then initialize the scene here.
    if (isInitialized() && scene != NULL)
        scene->init();
}

void SupportCanvas3D::copyPixels(int width, int height, BGRA *data)
{
    glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, data);
    cout << "copied " << width << "x" << height << endl;

    // Flip the image and since OpenGL uses an origin in the lower left and we an origin in the
    // upper left.
    for (int y = 0; y < (height + 1) / 2; y++)
        for (int x = 0; x < width; x++)
            std::swap(data[x + y * width], data[x + (height - y - 1) * width]);
}

void SupportCanvas3D::resetUpVector()
{
    // Reset the up vector to the y axis
    glm::vec4 up = glm::vec4(0.f, 1.f, 0.f, 0.f);
    if(NEQ(glm::length(m_defaultPerspectiveCamera->getUp() - up), 0))
    {
        m_defaultPerspectiveCamera->orientLook(
                    m_defaultPerspectiveCamera->getPosition(),
                    m_defaultPerspectiveCamera->getLook(),
                    up);
        update();
    }
}

void SupportCanvas3D::setCameraAxisX()
{
    m_defaultPerspectiveCamera->orientLook(
                glm::vec4(2.f, 0.f, 0.f, 1.f),
                glm::vec4(-1.f, 0.f, 0.f, 0.f),
                glm::vec4(0.f, 1.f, 0.f, 0.f));
    update();
}

void SupportCanvas3D::setCameraAxisY()
{
    m_defaultPerspectiveCamera->orientLook(
                glm::vec4(0.f, 2.f, 0.f, 1.f),
                glm::vec4(0.f, -1.f, 0.f, 0.f),
                glm::vec4(0.f, 0.f, 1.f, 0.f));
    update();
}

void SupportCanvas3D::setCameraAxisZ()
{
    m_defaultPerspectiveCamera->orientLook(
                glm::vec4(0.f, 0.f, 2.f, 1.f),
                glm::vec4(0.f, 0.f, -1.f, 0.f),
                glm::vec4(0.f, 1.f, 0.f, 0.f));
    update();
}

void SupportCanvas3D::setCameraAxonometric()
{
    m_defaultPerspectiveCamera->orientLook(
                glm::vec4(2.f, 2.f, 2.f, 1.f),
                glm::vec4(-1.f, -1.f, -1.f, 0.f),
                glm::vec4(0.f, 1.f, 0.f, 0.f));
    update();
}

void SupportCanvas3D::updateCameraHeightAngle()
{
    // The height angle is half the overall field of view of the camera
    m_defaultPerspectiveCamera->setHeightAngle(settings.cameraFov / 2);
}

void SupportCanvas3D::updateCameraTranslation()
{
    m_defaultPerspectiveCamera->translate(
            glm::vec4(
                settings.cameraPosX - m_oldPosX,
                settings.cameraPosY - m_oldPosY,
                settings.cameraPosZ - m_oldPosZ,
                0));

    m_oldPosX = settings.cameraPosX;
    m_oldPosY = settings.cameraPosY;
    m_oldPosZ = settings.cameraPosZ;
}

void SupportCanvas3D::updateCameraRotationU()
{
    m_defaultPerspectiveCamera->rotateU(settings.cameraRotU - m_oldRotU);
    m_oldRotU = settings.cameraRotU;
}

void SupportCanvas3D::updateCameraRotationV()
{
    m_defaultPerspectiveCamera->rotateV(settings.cameraRotV - m_oldRotV);
    m_oldRotV = settings.cameraRotV;
}

void SupportCanvas3D::updateCameraRotationN()
{
    m_defaultPerspectiveCamera->rotateW(settings.cameraRotN - m_oldRotN);
    m_oldRotN = settings.cameraRotN;
}

void SupportCanvas3D::updateCameraClip()
{
    m_defaultPerspectiveCamera->setClip(settings.cameraNear, settings.cameraFar);
}


void SupportCanvas3D::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        getCamera()->mouseDown(event->x(), event->y());
        m_isDragging = true;
        update();
    }
}

void SupportCanvas3D::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging)
    {
        getCamera()->mouseDragged(event->x(), event->y());
        update();
    }
}

void SupportCanvas3D::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isDragging && event->button() == Qt::RightButton)
    {
        getCamera()->mouseUp(event->x(), event->y());
        m_isDragging = false;
        update();
    }
}

void SupportCanvas3D::wheelEvent(QWheelEvent *event)
{
    getCamera()->mouseScrolled(event->delta());
    update();
}

void SupportCanvas3D::resizeEvent(QResizeEvent *event)
{
    emit aspectRatioChanged();
}
