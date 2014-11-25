#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"


RayScene::RayScene()
{
}

RayScene::~RayScene()
{
}

Vector4 RayScene::generateRay(Vector4 eye, Vector4 filmP, glm::mat4 M)
{
    Vector4 worldP = M * filmP;
    Vector4 tp = worldP - eye;
    Vector4 dir= glm::normalize(tp);

    pw = eye;
    dw = dir;
    return dir;
}


void RayScene::trace(Vector4 eye, Vector4 dir, BGRA &canvascolor)
{
    int intersectId = -1;

    CS123SceneColor flcolor= rayTrace(eye, dir, 0, intersectId);
    canvascolor.b = round(255.0 * flcolor.b);
    canvascolor.g = round(255.0 * flcolor.g);
    canvascolor.r = round(255.0 * flcolor.r);
}

CS123SceneColor RayScene::rayTrace(Vector4 eye, Vector4 dir, int recursiondepth, int intersectId)
{
    Vector3 tnormal(0, 0, 0); // normal at point t
    CS123SceneMaterial tmaterial;
    Vector2 tex(0,0);

//    std::cout<<recursiondepth<<"---------------------"<<endl;
//    std::cout<<intersectId<<endl;

    REAL t = calculateIntersection(eye,dir,tnormal,tmaterial,tex, intersectId);

//    std::cout<<intersectId<<endl;
//    std::cout<<t<<endl;

    CS123SceneColor flcolor;
    flcolor = flcolor * 0;

    if(t < MAX_LIMIT && recursiondepth < RECURSION_LIMIT)
    {
        flcolor = illuminatePoint(eye, dir, t, tnormal, tmaterial,tex,intersectId,recursiondepth);
    }
    return flcolor;
}

Vector3 RayScene::transNormalo2w(Vector3 normal,glm::mat4 M)
{
    Vector3 tp;

    tp.x = M[0][0]* normal.x + M[0][1]* normal.y + M[0][2]* normal.z;
    tp.y = M[1][0]* normal.x + M[1][1]* normal.y + M[1][2]* normal.z;
    tp.z = M[2][0]* normal.x + M[2][1]* normal.y + M[2][2]* normal.z;

    return glm::normalize(tp);
}

CS123SceneColor RayScene::illuminatePoint(Vector4 eye, Vector4 dir, REAL t, Vector3 n, CS123SceneMaterial matrl, Vector2 textureCo, int intersectId, int recursiondepth)
{
    CS123SceneColor tcolor;
    Vector3 pos_obj(eye.x + t*dir.x, eye.y + t*dir.y, eye.z + t*dir.z);
    Vector4 pos_objv4(pos_obj.x, pos_obj.y, pos_obj.z, 1);
    tcolor = tcolor*0;

    //ambient part
    tcolor.b = m_sceneglobalData.ka * matrl.cAmbient.b;
    tcolor.g = m_sceneglobalData.ka * matrl.cAmbient.g;
    tcolor.r = m_sceneglobalData.ka * matrl.cAmbient.r;

    //diffuse part and specular part for each light
    Vector3 viewray(-dir.x, -dir.y, -dir.z);
    viewray = glm::normalize(viewray);
    std::vector<CS123SceneLightData>::iterator it;
    for(it = m_scenelights.begin(); it != m_scenelights.end(); it ++)
    {
        if((*it).type == LIGHT_POINT && settings.usePointLights)
        {
            Vector3 lm ((*it).pos.x, (*it).pos.y, (*it).pos.z);
            lm = lm - pos_obj;
            REAL mag = glm::length(lm);
            lm = glm::normalize(lm);
            //whether in the shadow
            if(settings.useShadows)
            {
                if(isInShadow(pos_obj, lm, mag, intersectId))
                    continue;
            }

            REAL fatt = 1;
            Vector3 atr(0,0,0);
            if((*it).function != atr)
            {
                atr = (*it).function;
                fatt = 1/ (atr.x + atr.y * mag + atr.z * mag * mag);
            }

            //diffuse part (containing texture)
            Vector3 coeff1 = diffuseShade(lm,n,matrl,textureCo);
            tcolor.b += fatt * (*it).color.b * coeff1.x;
            tcolor.g += fatt * (*it).color.g * coeff1.y;
            tcolor.r += fatt * (*it).color.r * coeff1.z;

            //specular part
            Vector3 coeff2 = specularShade(lm,n,viewray,matrl);
            tcolor.b += fatt * (*it).color.b * coeff2.x;
            tcolor.g += fatt * (*it).color.g * coeff2.y;
            tcolor.r += fatt * (*it).color.r * coeff2.z;

        }
        if((*it).type == LIGHT_DIRECTIONAL && settings.useDirectionalLights)
        {
            Vector3 lightdirInverse (-(*it).dir.x, -(*it).dir.y, -(*it).dir.z);
            lightdirInverse = glm::normalize(lightdirInverse);
            if(settings.useShadows)
            {
                if(isInShadow(pos_obj, lightdirInverse, MAX_LIMIT, intersectId))
                    continue;
            }

            //diffuse part(containing texutre)
            Vector3 coeff1 = diffuseShade(lightdirInverse,n,matrl,textureCo);
            tcolor.b += (*it).color.b * coeff1.x;
            tcolor.g += (*it).color.g * coeff1.y;
            tcolor.r += (*it).color.r * coeff1.z;

            //specular part
            Vector3 coeff2 = specularShade(lightdirInverse,n,viewray,matrl);
            tcolor.b += (*it).color.b * coeff2.x;
            tcolor.g += (*it).color.g * coeff2.y;
            tcolor.r += (*it).color.r * coeff2.z;
        }
    }

    //reflected part
    if(settings.useReflection && matrl.cReflective.b != 0 && matrl.cReflective.g != 0 && matrl.cReflective.r != 0)
    {
        Vector3 rflvr= - glm::reflect(viewray, n);
        Vector4 rflvrv4 = glm::normalize(Vector4(rflvr.x, rflvr.y, rflvr.z, 0));
        CS123SceneColor nextPointColor = rayTrace(pos_objv4,rflvrv4,++recursiondepth, intersectId);

        tcolor.b += m_sceneglobalData.ks * matrl.cReflective.b * nextPointColor.b;
        tcolor.g += m_sceneglobalData.ks * matrl.cReflective.g * nextPointColor.g;
        tcolor.r += m_sceneglobalData.ks * matrl.cReflective.r * nextPointColor.r;
    }

    return tcolor;
}

bool RayScene::isInShadow(Vector3 object, Vector3 raydir, REAL tmin, int intersectId)
{
    Vector4 objectv4(object.x, object.y, object.z, 1);
    Vector4 shadowray(raydir.x, raydir.y, raydir.z, 0);

    Vector3 tnormal(0, 0, 0); // normal at point t
    CS123SceneMaterial tmaterial;
    Vector2 tex(0,0);
    REAL t = calculateIntersection(objectv4,shadowray,tnormal,tmaterial,tex, intersectId);
    if( t < tmin)
        return true;
    else
        return false;
}

REAL RayScene::calculateIntersection(Vector4 start, Vector4 dir, Vector3& normal, CS123SceneMaterial& tmaterial, Vector2 &textureCo, int &intersectId)
{
    Shape* m_vshape = NULL;
    glm::mat4 Mw2o;
    std::vector<primitiveNmatrix>::iterator it;
    Vector3 tnormal(0,0,0);
    REAL tpostmin = MAX_LIMIT;
    int tpId = -1;
    REAL t = MAX_LIMIT; // to compare
    textureCo = Vector2(0,0);

    for(it = m_tbd.begin(); it != m_tbd.end(); it++)
    {
        if((*it).shapetype!= PRIMITIVE_CUBE &&(*it).shapetype!= PRIMITIVE_CONE &&(*it).shapetype!= PRIMITIVE_CYLINDER && (*it).shapetype!= PRIMITIVE_SPHERE)
            continue;
        if((*it).id == intersectId)
            continue;
        Mw2o = glm::inverse((*it).comMatrix);
        p = Mw2o*start;
        d = Mw2o*dir;

        switch((*it).shapetype)
        {
        case PRIMITIVE_CUBE:
            m_vshape = new Cube(p,d);
            break;
        case PRIMITIVE_CONE:
            m_vshape = new Cone(p,d);
            break;
        case PRIMITIVE_CYLINDER:
            m_vshape = new Cylinder(p,d);
            break;
        case PRIMITIVE_SPHERE:
            m_vshape = new Sphere(p,d);
            break;
        default:
            break;
        }

        if(m_vshape != NULL)
        {
            t = m_vshape->calculateIntersecP(tnormal,textureCo);
            delete m_vshape;
            m_vshape = NULL;
        }

        if(t>1E-6 && t < tpostmin)
        {
            tpostmin = t;
            normal = transNormalo2w(tnormal,glm::inverse((*it).comMatrix));
            tmaterial = (*it).material;
            tpId = (*it).id;
        }
    }
    intersectId = tpId;
    return tpostmin;
}

Vector3 RayScene::diffuseShade(Vector3 lightray, Vector3 n, CS123SceneMaterial matrl, Vector2 textureCo)
{
    Vector3 coeff(0,0,0);

    //diffuse part
    REAL dotproduct = glm::dot(lightray, n);
    if(dotproduct > 0)
    {
        CS123SceneColor mixcolor = matrl.cDiffuse;
        if(settings.useTextureMapping)
             mixcolor = textureShade(matrl,textureCo,matrl.cDiffuse);

        coeff.x = m_sceneglobalData.kd * mixcolor.b * dotproduct;
        coeff.y = m_sceneglobalData.kd * mixcolor.g * dotproduct;
        coeff.z = m_sceneglobalData.kd * mixcolor.r * dotproduct;
    }

    return coeff;
}

Vector3 RayScene::specularShade(Vector3 lightray, Vector3 n, Vector3 viewray, CS123SceneMaterial matrl)
{
    Vector3 coeff(0,0,0);

    //specular part
    Vector3 reflectedray = - glm::reflect(lightray, n);
    reflectedray = glm::normalize(reflectedray);
    REAL spedotproduct = glm::dot(reflectedray, viewray);
    if(spedotproduct > 0)
    {
        CS123SceneColor mixcolor = matrl.cSpecular;

        spedotproduct = pow(spedotproduct,matrl.shininess);

        coeff.x = m_sceneglobalData.ks * mixcolor.b * spedotproduct;
        coeff.y = m_sceneglobalData.ks * mixcolor.g * spedotproduct;
        coeff.z = m_sceneglobalData.ks * mixcolor.r * spedotproduct;
    }
    return coeff;
}

CS123SceneColor RayScene::textureShade(CS123SceneMaterial matrl, Vector2 textureCo, CS123SceneColor tcolor)
{
    CS123SceneColor mcl;
    if(matrl.textureMap->isUsed)
    {
        QImage tp = texImgPair[matrl.textureMap->filename];

        int idxs, idxt;
        idxs = ((int)(textureCo.x*matrl.textureMap->repeatU*tp.width()))%tp.width();
        idxt = ((int)(textureCo.y*matrl.textureMap->repeatV*tp.height()))%tp.height();

        QRgb texcolor = tp.pixel(idxs, idxt);

        mcl.b = (1-matrl.blend)*tcolor.b + matrl.blend * float(qBlue(texcolor))/255.0f;
        mcl.g = (1-matrl.blend)*tcolor.g + matrl.blend * float(qGreen(texcolor))/255.0f;
        mcl.r = (1-matrl.blend)*tcolor.r + matrl.blend * float(qRed(texcolor))/255.0f;

        return mcl;
    }
    else
        return tcolor;

}

void RayScene::setTextureImage()
{
    std::vector<primitiveNmatrix>::iterator it;

    for(it = m_tbd.begin(); it != m_tbd.end(); it++)
    {
        if((*it).material.textureMap->isUsed)
        {
            std::string filen = (*it).material.textureMap->filename;
            if(texImgPair.find(filen) == texImgPair.end())
            {
                QImage *tp = new QImage();

                tp->load(QString::fromStdString(filen));
                texImgPair[(*it).material.textureMap->filename] = *tp;
                delete tp;
            }
        }
    }
}

 void RayScene::builKdtree()
 {
     //root AABB range
     m_root = new KdtreeNode();
     std::vector<primitiveNmatrix>::iterator it;
     for(it = m_tbd.begin(); it != m_tbd.end(); it++)
     {
         primitiveNmatrix pnm = (*it);
         
         pnm.xRange.x = MAX_LIMIT;
         pnm.xRange.y = - MAX_LIMIT;
         pnm.yRange.x = MAX_LIMIT;
         pnm.yRange.y = - MAX_LIMIT;
         pnm.zRange.x = MAX_LIMIT;
         pnm.zRange.y = - MAX_LIMIT;
         
         Vector4 v1(-0.5, -0.5, -0.5, 1);
         Vector4 v2( 0.5, -0.5, -0.5, 1);
         Vector4 v3(-0.5,  0.5, -0.5, 1);
         Vector4 v4( 0.5,  0.5, -0.5, 1);
         Vector4 v5(-0.5, -0.5,  0.5, 1);
         Vector4 v6( 0.5, -0.5,  0.5, 1);
         Vector4 v7(-0.5,  0.5,  0.5, 1);
         Vector4 v8( 0.5,  0.5,  0.5, 1);
         
         v1 = pnm.comMatrix * v1;
         v2 = pnm.comMatrix * v2;
         v3 = pnm.comMatrix * v3;
         v4 = pnm.comMatrix * v4;
         v5 = pnm.comMatrix * v5;
         v6 = pnm.comMatrix * v6;
         v7 = pnm.comMatrix * v7;
         v8 = pnm.comMatrix * v8;

         calculateAABB(v1, pnm.xRange, pnm.yRange, pnm.zRange);
         calculateAABB(v2, pnm.xRange, pnm.yRange, pnm.zRange);
         calculateAABB(v3, pnm.xRange, pnm.yRange, pnm.zRange);
         calculateAABB(v4, pnm.xRange, pnm.yRange, pnm.zRange);
         calculateAABB(v5, pnm.xRange, pnm.yRange, pnm.zRange);
         calculateAABB(v6, pnm.xRange, pnm.yRange, pnm.zRange);
         calculateAABB(v7, pnm.xRange, pnm.yRange, pnm.zRange);
         calculateAABB(v8, pnm.xRange, pnm.yRange, pnm.zRange);

         //for the root node
         calculateAABB(v1, m_root->xRange, m_root->yRange, m_root->zRange);
         calculateAABB(v2, m_root->xRange, m_root->yRange, m_root->zRange);
         calculateAABB(v3, m_root->xRange, m_root->yRange, m_root->zRange);
         calculateAABB(v4, m_root->xRange, m_root->yRange, m_root->zRange);
         calculateAABB(v5, m_root->xRange, m_root->yRange, m_root->zRange);
         calculateAABB(v6, m_root->xRange, m_root->yRange, m_root->zRange);
         calculateAABB(v7, m_root->xRange, m_root->yRange, m_root->zRange);
         calculateAABB(v8, m_root->xRange, m_root->yRange, m_root->zRange);
     }


 }

 void RayScene::calculateAABB(Vector4 v, Vector2& xRange, Vector2& yRange, Vector2& zRange)
 {
     if(v.x > pnm.xRange.y)
         xRange.y = v.x;
     if(v.x < pnm.xRange.x)
         xRange.x = v.x;

     if(v.y > pnm.yRange.y)
         yRange.y = v.y;
     if(v.y < pnm.yRange.x)
         yRange.x = v.y;

     if(v.z > pnm.zRange.y)
         zRange.y = v.z;
     if(v.z < pnm.zRange.x)
         zRange.x = v.z;
 }
