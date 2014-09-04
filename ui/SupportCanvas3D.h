#ifndef SUPPORTCANVAS3D_H
#define SUPPORTCANVAS3D_H

#include "GL/glew.h"
#include <QGLWidget>
#include <CS123Common.h>

class Camera;
class OpenGLScene;
class ShapesScene;
class SceneviewScene;
class OrbitingCamera;
class CamtransCamera;

/**
 * @class  SupportCanvas3D
 *
 * The SupportCanvas3D class holds a single active OpenGLScene, and either
 * calls upon that scene to draw itself using OpenGL or draws the scene
 * by directly calling upon OpenGL (getting the scene-specific information
 * from the OpenGLScene object). The details of the implementation are left
 * to the student; neither way is better than the other.
 *
 * The SupportCanvas3D also contains a default camera which can be used in
 * case the loaded scene does not specify a camera.
 */
class SupportCanvas3D : public QGLWidget
{
    Q_OBJECT
public:
    SupportCanvas3D(QGLFormat format, QWidget *parent);
    virtual ~SupportCanvas3D();

    // Returns a pointer to this Canvas3D's default camera, based on the application settings.
    Camera *getCamera();

    // Returns a pointer to the orbiting camera.
    OrbitingCamera *getOrbitingCamera();

    // Returns a pointer to the camtrans camera.
    CamtransCamera *getCamtransCamera();

    // Returns a pointer to the current scene. If no scene is loaded, this function returns NULL.
    OpenGLScene *getScene() { return m_scene; }

    // Sets the current scene. This class does takes ownership of the specified scene.
    // (You are NOT responsible for deleting it).
    void setScene(OpenGLScene *scene);

    // Copies pixels from the OpenGL render buffer into a standard bitmap image, using row-major
    // order and BGRA data format.
    void copyPixels(int width, int height, BGRA *data);

    // This function will be called by the UI when the settings have changed.
    virtual void settingsChanged();

    // Returns true if initializeGL has been called.
    bool isInitialized();

public slots:
    // These will be called by the corresponding UI buttons on the Camtrans dock
    void resetUpVector();
    void setCameraAxisX();
    void setCameraAxisY();
    void setCameraAxisZ();
    void setCameraAxonometric();

    // These will be called whenever the corresponding UI elements are updated on the Camtrans dock
    void updateCameraHeightAngle();
    void updateCameraTranslation();
    void updateCameraRotationU();
    void updateCameraRotationV();
    void updateCameraRotationN();
    void updateCameraClip();

signals:
    void aspectRatioChanged();

protected:
    // Overridden from QGLWidget
    virtual void initializeGL() = 0;
    virtual void paintGL() = 0;

    // Overridden from QWidget
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    float m_oldPosX, m_oldPosY, m_oldPosZ;
    float m_oldRotU, m_oldRotV, m_oldRotN;

    bool m_initialized;

private:
    glm::vec4      m_cameraEye;
    bool           m_isDragging;

    CamtransCamera *m_defaultPerspectiveCamera;
    OrbitingCamera *m_defaultOrbitingCamera;
    OpenGLScene    *m_scene;
};

#endif // SUPPORTCANVAS3D_H
