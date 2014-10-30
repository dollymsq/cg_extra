/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
    m_near = 1;
    m_far = 30;
    m_heightAngle = 60;
    m_aspectRatio = 1;

    orientLook(Vector4(2, 2, 2, 1), Vector4(-1, -1, -1, 0), Vector4(0, 1, 0, 0));

    updateProjectionMatrix();
}

void CamtransCamera::setAspectRatio(float a)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_aspectRatio = a;

    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_projectionMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_viewMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_scale;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_perUnhinge;
}

glm::vec4 CamtransCamera::getPosition() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_eyePoint;
}

glm::vec4 CamtransCamera::getLook() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Vector4(0,0,0,0) - w;
}

glm::vec4 CamtransCamera::getUp() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return v;
}

float CamtransCamera::getAspectRatio() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_heightAngle;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_eyePoint = eye;
    w = Vector4(0, 0, 0, 0)-look;
    w = glm::normalize(w);
    Vector3 tp = glm::cross(Vector3(up), Vector3(w));
    u = glm::normalize(Vector4(tp, 0));
    v = Vector4(glm::cross(Vector3(w), Vector3(u)), 0);

    updateViewMatrix();
}

void CamtransCamera::setHeightAngle(float h)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_heightAngle = h;

    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_eyePoint += v;

    updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...

    Vector4 v0 = Vector4(v);
    Vector4 w0 = Vector4(w);
    v = v0 * float(cos(degrees/180.0*M_PI)) + w0 * float(sin(degrees/180.0*M_PI));
    w = -v0 * float(sin(degrees/180.0*M_PI)) + w0 * float(cos(degrees/180.0*M_PI));
    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...

    Vector4 u0 = Vector4(u);
    Vector4 w0 = Vector4(w);
    u = u0 *float(cos(degrees/180*M_PI)) - w0* float(sin(degrees/180*M_PI));
    w = u0 * float(sin(degrees/180*M_PI)) + w0 * float(cos(degrees/180*M_PI));

    updateViewMatrix();
}

void CamtransCamera::rotateW(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...
//    glm::mat4x4 M = getRotMat(m_eyePoint, w, degrees/180*M_PI);
//    v = M * v;
//    u = M * u;
    Vector4 u0 = Vector4(u);
    Vector4 v0 = Vector4(v);
    u = v0 * float(sin(degrees/180*M_PI)) + u0 * float(cos(degrees/180*M_PI));
    v = v0 * float(cos(degrees/180.0*M_PI)) - u0 * float(sin(degrees/180.0*M_PI));

    updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_near = nearPlane;
    m_far = farPlane;

    updateProjectionMatrix();
}

void CamtransCamera::updateProjectionMatrix()
{

    REAL c= -m_near/m_far;
    m_perUnhinge = glm::mat4x4(1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, -1/(c+1), c/(c+1),
                             0, 0, -1, 0);
    m_perUnhinge = glm::transpose(m_perUnhinge);

    REAL scaley = 1/(m_far*tan(m_heightAngle/360*M_PI));
    REAL scalex = scaley / m_aspectRatio;
    m_scale = glm::mat4x4(scalex, 0, 0, 0,
                        0, scaley, 0, 0,
                        0, 0, 1/m_far, 0,
                        0, 0, 0, 1);
    m_scale = glm::transpose(m_scale);

    m_projectionMatrix =  m_perUnhinge * m_scale;
}

void CamtransCamera::updateViewMatrix()
{
    glm::mat4x4 MRot(u.x, u.y, u.z, 0,
                   v.x, v.y, v.z, 0,
                   w.x, w.y, w.z, 0,
                   0, 0, 0, 1);
    MRot = glm::transpose(MRot);
    glm::mat4x4 MTrans(1, 0, 0, -m_eyePoint.x,
                     0, 1, 0, -m_eyePoint.y,
                     0, 0, 1, -m_eyePoint.z,
                     0, 0, 0, 1);
    MTrans = glm::transpose(MTrans);
    m_viewMatrix = MRot * MTrans;
}
