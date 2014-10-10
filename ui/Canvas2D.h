#ifndef CANVAS2D_H
#define CANVAS2D_H

#include "SupportCanvas2D.h"

#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include "brush/TubeBrush.h"

#include "filter/Filter.h"
#include "filter/FInvert.h"
#include "filter/FGrayScale.h"
#include "filter/FEdgeDetect.h"
#include "filter/FBlur.h"
#include "filter/FScale.h"
#include "filter/FRotate.h"
#include "filter/FAutoLevel.h"
#include "filter/FMedian.h"

#include "camera/Camera.h"
#include "camera/CamtransCamera.h"
#include "camera/OrbitingCamera.h"

class RayScene;

class Camera;

/**
 * @class Canvas2D
 *
 * 2D canvas that students will implement in the Brush and Filter assignments. The same canvas
 * will be used to display raytraced images in the Intersect and Ray assignments.
 */
class Canvas2D : public SupportCanvas2D
{
    Q_OBJECT
public:
    Canvas2D();
    virtual ~Canvas2D();

    void setScene(RayScene *scene);

    // UI will call this from the button on the "Ray" dock
    void renderImage(Camera *camera, int width, int height);

    // This will be called when the settings have changed
    virtual void settingsChanged();


public slots:
    // UI will call this from the button on the "Ray" dock
    void cancelRender();

    // UI will call this from the button on the "Filter" dock
    void filterImage();

protected:
    virtual void paintEvent(QPaintEvent *);  // Overridden from SupportCanvas2D.
    virtual void mouseDown(int x, int y);    // Called when left mouse button is pressed on canvas
    virtual void mouseDragged(int x, int y); // Called when left mouse button is dragged on canvas
    virtual void mouseUp(int x, int y);      // Called when left mouse button is released

    // Called when the size of the canvas has been changed
    virtual void notifySizeChanged(int w, int h);



private:
    RayScene *m_scene;

    //TODO: [BRUSH, INTERSECT, RAY] Put your member variables here.

    //[BRUSH]
    Brush *m_b;
    BGRA *m_canvas_ori;
    float *m_canvas_tmp;
    Filter *m_filter;
    int previousBrush;

};

#endif // CANVAS2D_H
