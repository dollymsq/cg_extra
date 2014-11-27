/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include "Canvas2D.h"
#include "Canvas3D.h"



#include <QPainter>

Canvas2D::Canvas2D()
{
    // @TODO: Initialize any pointers in this class here.
    m_scene = NULL;
    m_b = NULL;
    m_canvas_ori = NULL;
    m_canvas_tmp = NULL;
    m_filter = NULL;
}

Canvas2D::~Canvas2D()
{
    // @TODO: Be sure to release all memory that you allocate.
    if (m_scene != NULL)
        delete m_scene;
    if (m_b != NULL)
        delete m_b;
    if (m_filter != NULL)
        delete m_filter;

}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {

}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.

    SupportCanvas2D::paintEvent(e);

}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************

void Canvas2D::mouseDown(int x, int y)
{
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    int currentBrush = settings.brushType;
    int currentRadius = settings.brushRadius;

    BGRA currentColor;
    currentColor.b = settings.brushBlue;
    currentColor.g = settings.brushGreen;
    currentColor.r = settings.brushRed;
    currentColor.a = 255;

    int currentFlow = settings.brushAlpha;
    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

    if(m_b == NULL)
    {
        switch(currentBrush)
        {
        case BRUSH_SOLID:
            m_b = new ConstantBrush(currentColor, currentFlow, currentRadius);
            break;
        case BRUSH_LINEAR:
            m_b = new LinearBrush(currentColor, currentFlow, currentRadius);
            break;
        case BRUSH_QUADRATIC:
            m_b = new QuadraticBrush(currentColor, currentFlow, currentRadius);
            break;
        case BRUSH_SMUDGE:
            m_b = new SmudgeBrush(currentColor, currentFlow, currentRadius);
            break;
        case BRUSH_TUBE:
            m_b = new TubeBrush(currentColor, currentFlow, currentRadius);
            break;
        default:
            break;
        }
    }
    else
    {
        if(fixAlphaBlending)
        {
            int size = width() * height();
            m_canvas_ori = new BGRA[size];
            m_canvas_tmp = new float[size];
            memcpy(m_canvas_ori, data(), size * sizeof(int));// the original state of the the canvas before the stroke
            memset(m_canvas_tmp, 0, size * sizeof(int));
            m_b->paintOnCanvasTmp(x, y, this, m_canvas_tmp);
            m_b->paintOnce(x,y,this);
        }
        else
        {
            if(currentBrush != BRUSH_SMUDGE)
                m_b->paintOnce(x, y, this);
        }

    }

}

void Canvas2D::mouseDragged(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.
    if(settings.fixAlphaBlending)
    {
        switch(settings.brushType)
        {
        case(BRUSH_SOLID):

        case(BRUSH_LINEAR):

        case(BRUSH_QUADRATIC):
            m_b->paintOnCanvasTmp(x, y, this, m_canvas_tmp);
            m_b->fixAlphaBlendingF(this, m_canvas_ori, m_canvas_tmp);
            break;
        case(BRUSH_TUBE):

        case(BRUSH_SMUDGE):
            m_b->paintOnce(x, y, this);
            break;
        default:
            break;
        }
    }
    else
    {
        switch(settings.brushType)
        {
        case(BRUSH_SOLID):

        case(BRUSH_LINEAR):

        case(BRUSH_QUADRATIC):

        case(BRUSH_TUBE):

        case(BRUSH_SMUDGE):
            m_b->paintOnce(x, y, this);
            break;
        default:
            break;
        }
    }

}

void Canvas2D::mouseUp(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.
    previousBrush = settings.brushType;
    if(!settings.fixAlphaBlending)
    {}
    else
    {
        if (m_canvas_ori != NULL)
        {
            delete[] m_canvas_ori;
            delete[] m_canvas_tmp;
        }
    }
    if(settings.brushType == BRUSH_SMUDGE)
        m_b->clearPaint();
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage()
{
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.

    switch (settings.filterType) {
    case FILTER_BLUR:
        m_filter = new FBlur();
        m_filter->setBlurRadius(settings.blurRadius);
        break;
    case FILTER_INVERT:
        m_filter = new FInvert();
        break;
    case FILTER_GRAYSCALE:
        m_filter = new FGrayScale();
        break;
    case FILTER_EDGE_DETECT:
        m_filter = new FEdgeDetect();
        m_filter->setThreshold(settings.edgeDetectThreshold);
        break;
    case FILTER_SCALE:
        m_filter = new FScale();
        m_filter->setScaleXnY(settings.scaleX, settings.scaleY);
        break;
    case FILTER_ROTATE:
        m_filter = new FRotate();
        m_filter->setRotateDegree(settings.rotateAngle);
        break;
    case FILTER_MEDIAN:
        m_filter = new FMedian();
        break;
    case FILTER_BILATERAL:
        m_filter = new FBilateral();
        break;
    case FILTER_SPECIAL_3:
        m_filter = new FAutoLevel();
        break;
    default:
        //to be continued;
        m_filter = NULL;
        break;
    }
    if(m_filter!= NULL)
    {
        m_filter->setRegionParameter(this);//set the region to handle
        m_filter->doFilter(this->data());
    }
    //external handling of special filter
    if(settings.filterType == FILTER_SCALE || settings.filterType == FILTER_ROTATE)
        m_filter->setCanvas(this);
    update();
}

void Canvas2D::setScene(RayScene *scene)
{
    delete m_scene;
    m_scene = scene;
}


void Canvas2D::renderImage(Camera *camera, int width, int height)
{
    if (m_scene)
    {
        this->resize(width,height);
        Vector4 eye(0,0,0,1);

        Vector4 filmP(0,0,-1,1);
        glm::mat4 Mc2w = camera->getScaleMatrix() * camera->getViewMatrix();
        Mc2w = glm::inverse(Mc2w);
        Vector4 tp = Mc2w * eye;
        eye = tp;

        if(settings.useTextureMapping)
            m_scene->setTextureImage();

        m_scene->builKdtree();
        std::cout<< "kdtree built"<<endl;

        if(settings.useSuperSampling)
        {
            Vector4 filmP1(0,0,-1,1);
            Vector4 filmP2(0,0,-1,1);
            Vector4 filmP3(0,0,-1,1);
            Vector4 filmP4(0,0,-1,1);

            for(int i = 0; i< height; i++)
            {
                for(int j = 0; j< width; j++)
                {
                    if(i == 137 && j == 298)
                    {
                        tp = filmP;
                        //                    return;
                    }
                    filmP.x = 2.0*(float)j/(float)width-1.0;
                    filmP.y = 1.0-2.0*(float)i/(float)height;

                    filmP1.x = 2.0*(float)(j - 0.5)/(float)width-1.0;
                    filmP1.y = 1.0-2.0*(float)(i - 0.5)/(float)height;

                    filmP2.x = 2.0*(float)(j - 0.5)/(float)width-1.0;
                    filmP2.y = 1.0-2.0*(float)(i + 0.5)/(float)height;

                    filmP3.x = 2.0*(float)(j + 0.5)/(float)width-1.0;
                    filmP3.y = 1.0-2.0*(float)(i + 0.5)/(float)height;

                    filmP4.x = 2.0*(float)(j + 0.5)/(float)width-1.0;
                    filmP4.y = 1.0-2.0*(float)(i - 0.5)/(float)height;

                    CS123SceneColor flcolor= superSample(eye, Mc2w, j-0.5, j+0.5, i-0.5, i+0.5, 0);
                    BGRA canvascolor;
                    canvascolor.b = round(255.0 * flcolor.b);
                    canvascolor.g = round(255.0 * flcolor.g);
                    canvascolor.r = round(255.0 * flcolor.r);

                    data()[i*width + j] = canvascolor;
               }
            }
        }
        else
        {
            for(int i = 0; i< height; i++)
            {
                for(int j = 0; j< width; j++)
                {
                    //screen to film done(camera)
                    filmP.x = 2.0*(float)j/(float)width-1.0;
                    filmP.y = 1.0-2.0*(float)i/(float)height;

                    //to generate ray in world space, the filmP will corresponds to d, copy that
                    Vector4 dir = m_scene->generateRay(eye, filmP, Mc2w);

                    //calculate intersection point if any and its corresponding normal
                    //                if(i == 297 && j == 484)
                    //                if(i == 214 && j == 123)
                    if(i == 137 && j == 298)
                    {
                        tp = filmP;
                        //                    return;
                    }
                    //                std::cout<<"Image pos"<< j<<", "<<i<<"========================="<<endl;

                    CS123SceneColor flcolor= m_scene->trace(eye,dir);
                    BGRA canvascolor;
                    canvascolor.b = round(255.0 * flcolor.b);
                    canvascolor.g = round(255.0 * flcolor.g);
                    canvascolor.r = round(255.0 * flcolor.r);

                    data()[i*width + j] = canvascolor;
                }
            }
        }

        if(settings.useAntiAliasing)
        {
            FEdgeDetect* edgeblur = new FEdgeDetect();
            edgeblur->setRegionParameter(this);//set the region to handle
            edgeblur->setThreshold(0.5);
            edgeblur->blurEdges(data());

            delete edgeblur;
        }
    }
}

CS123SceneColor Canvas2D::superSample(const Vector4 &eye, const glm::mat4 &Mc2w, double xmin, double xmax, double ymin, double ymax, int depth)
{
    CS123SceneColor color1, color2, color3, color4, colorct;

    Vector4 filmP(0,0,-1,1);
    Vector4 filmP1(0,0,-1,1);
    Vector4 filmP2(0,0,-1,1);
    Vector4 filmP3(0,0,-1,1);
    Vector4 filmP4(0,0,-1,1);

    filmP.x = (2.0 * (ymin+ymax)/2.0 / width()) - 1.0;
    filmP.y = 1.0 - (2.0 * (xmin+xmax)/2.0 / height());

    filmP.x = (2.0 * (ymin+ymax)/2.0 / width()) - 1.0;
    filmP.y = 1.0 - (2.0 * (xmin+xmax)/2.0 / height());

    filmP1.x = (2.0 * ymin/ (double)width())-1.0;
    filmP1.y = 1.0 - (2.0*xmin/(double)height());

    filmP2.x = 2.0 * ymin / (double)width() -1.0;
    filmP2.y = 1.0-2.0*xmax/ (double)height();

    filmP3.x = 2.0*ymax/width()-1.0;
    filmP3.y = 1.0-2.0*xmax/height();

    filmP4.x = 2.0*ymax/width()-1.0;
    filmP4.y = 1.0-2.0*xmin/height();

    Vector4 dir1 = m_scene->generateRay(eye, filmP1, Mc2w);
    Vector4 dir2 = m_scene->generateRay(eye, filmP2, Mc2w);
    Vector4 dir3 = m_scene->generateRay(eye, filmP3, Mc2w);
    Vector4 dir4 = m_scene->generateRay(eye, filmP4, Mc2w);
    Vector4 dir = m_scene->generateRay(eye, filmP, Mc2w);

    color1 = m_scene->trace(eye,dir1);
    color2 = m_scene->trace(eye,dir2);
    color3 = m_scene->trace(eye,dir3);
    color4 = m_scene->trace(eye,dir4);
    colorct = m_scene->trace(eye,dir);

    if((color1.dist(colorct) < 0.2 && color2.dist(colorct)  < 0.2
            && color3.dist(colorct) < 0.2 && color4.dist(colorct) < 0.2)
            || depth > sqrt(settings.numSuperSamples))
    {
        return (color1 + color2 + color3 + color4)*(1/8) + colorct*(1/2);
    }
    else
    {
        if((color1.dist(colorct) >= 0.2 ))
            color1 = superSample(eye, Mc2w, xmin, (xmin + xmax)/2, ymin, (ymin+ymax)/2, depth + 1);
        if((color2.dist(colorct) >= 0.2 ))
            color2 = superSample(eye, Mc2w, (xmin + xmax)/2, xmax, ymin, (ymin+ymax)/2, depth + 1);
        if((color3.dist(colorct) >= 0.2 ))
            color3 = superSample(eye, Mc2w, (xmin + xmax)/2, xmax, (ymin+ymax)/2, ymax, depth + 1);
        if((color4.dist(colorct) >= 0.2 ))
            color4 = superSample(eye, Mc2w, xmin, (xmin + xmax)/2, (ymin+ymax)/2, ymax, depth + 1);
        return (color1 + color2 + color3 + color4)*(1/8) + colorct*(1/2);
    }
}

void Canvas2D::cancelRender()
{
    // TODO: cancel the raytracer (optional)
}


void Canvas2D::settingsChanged() {

    // TODO: Process changes to the application settings.
    int currentBrush = settings.brushType;
    int currentRadius = settings.brushRadius;

    BGRA currentColor;
    currentColor.b = settings.brushBlue;
    currentColor.g = settings.brushGreen;
    currentColor.r = settings.brushRed;
    currentColor.a = 255;

    int currentFlow = settings.brushAlpha;

    if(m_b != NULL && previousBrush != currentBrush)
    {
        delete m_b;
        m_b = NULL;
    }
    if(m_b == NULL)
    {
        switch(currentBrush)
        {
        case(BRUSH_SOLID):
            m_b = new ConstantBrush(currentColor, currentFlow, currentRadius);
            break;
        case(BRUSH_LINEAR):
            m_b = new LinearBrush(currentColor, currentFlow, currentRadius);
            break;
        case(BRUSH_QUADRATIC):
            m_b = new QuadraticBrush(currentColor, currentFlow, currentRadius);
            break;
        case(BRUSH_SMUDGE):
            m_b = new SmudgeBrush(currentColor, currentFlow, currentRadius);
            break;
        case(BRUSH_TUBE):
            m_b = new TubeBrush(currentColor, currentFlow, currentRadius);
            break;
        default:
            break;
        }
    }
    else
    {
        if(currentBrush == BRUSH_SMUDGE)
        {
            m_b->setFlow(currentFlow);
            m_b->setRadius(currentRadius);
        }
        else
        {
            if(currentBrush == BRUSH_TUBE)
                m_b->clearPaint();
            m_b->setBlue(currentColor.b);
            m_b->setGreen(currentColor.g);
            m_b->setRed(currentColor.r);
            m_b->setRadius(currentRadius);
            m_b->setFlow(currentFlow);
        }
    }
    previousBrush = currentBrush;
}

