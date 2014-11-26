#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"


RayScene::RayScene()
{
}

RayScene::~RayScene()
{
    clear(m_root);
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

    REAL t = MAX_LIMIT;

    traverseTree(m_root,t, eye,dir,tnormal,tmaterial,tex, intersectId);
//    calculateIntersection(eye,dir,tnormal,tmaterial,tex, intersectId);

//    std::cout<<intersectId<<endl;
//    std::cout<<t<<endl;

    CS123SceneColor flcolor;
    flcolor = flcolor * 0;

    if(t < MAX_LIMIT && recursiondepth < RECURSION_LIMIT)
    {
        flcolor = illuminatePoint(eye, dir, t, tnormal, tmaterial,tex,intersectId,recursiondepth);
    }
    flcolor.threshold();
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
    REAL t = MAX_LIMIT;
//    calculateIntersection(t, objectv4,shadowray,tnormal,tmaterial,tex, intersectId);
    traverseTree(m_root, t, objectv4, shadowray,tnormal,tmaterial,tex, intersectId);
    if( t < tmin)
        return true;
    else
        return false;
}

void RayScene::calculateIntersection(REAL &tpostmin, std::vector<primitiveNmatrix*> node_tbd, Vector4 start, Vector4 dir, Vector3& normal, CS123SceneMaterial& tmaterial, Vector2 &textureCo, int &intersectId)
{
    Shape* m_vshape = NULL;
    glm::mat4 Mw2o;
    std::vector<primitiveNmatrix*>::iterator it;
    Vector3 tnormal(0,0,0);
//    REAL tpostmin = MAX_LIMIT;
    int tpId = intersectId;
    REAL t = MAX_LIMIT; // to compare
    textureCo = Vector2(0,0);

    for(it = node_tbd.begin(); it != node_tbd.end(); it++)
    {
        if((*it)->shapetype!= PRIMITIVE_CUBE &&(*it)->shapetype!= PRIMITIVE_CONE &&(*it)->shapetype!= PRIMITIVE_CYLINDER && (*it)->shapetype!= PRIMITIVE_SPHERE)
            continue;
        if((*it)->id == intersectId)
            continue;
        Mw2o = glm::inverse((*it)->comMatrix);
        p = Mw2o*start;
        d = Mw2o*dir;

        switch((*it)->shapetype)
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
            normal = transNormalo2w(tnormal,glm::inverse((*it)->comMatrix));
            tmaterial = (*it)->material;
            tpId = (*it)->id;
        }
    }
    intersectId = tpId;
//    m_intersectId = tpId;
//    return tpostmin;
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
     //find each primitive's bounding box
     m_root = new KdtreeNode();
     m_root->xRange.x = MAX_LIMIT;
     m_root->xRange.y = - MAX_LIMIT;
     m_root->yRange.x = MAX_LIMIT;
     m_root->yRange.y = - MAX_LIMIT;
     m_root->zRange.x = MAX_LIMIT;
     m_root->zRange.y = - MAX_LIMIT;
     std::vector<primitiveNmatrix>::iterator it;
     for(it = m_tbd.begin(); it != m_tbd.end(); it++)
     {
         primitiveNmatrix *pnm = &(*it);
         m_root->m_tbd.push_back(pnm);

         pnm->xRange.x = MAX_LIMIT;
         pnm->xRange.y = - MAX_LIMIT;
         pnm->yRange.x = MAX_LIMIT;
         pnm->yRange.y = - MAX_LIMIT;
         pnm->zRange.x = MAX_LIMIT;
         pnm->zRange.y = - MAX_LIMIT;
         
         Vector4 v1(-0.5, -0.5, -0.5, 1);
         Vector4 v2( 0.5, -0.5, -0.5, 1);
         Vector4 v3(-0.5,  0.5, -0.5, 1);
         Vector4 v4( 0.5,  0.5, -0.5, 1);
         Vector4 v5(-0.5, -0.5,  0.5, 1);
         Vector4 v6( 0.5, -0.5,  0.5, 1);
         Vector4 v7(-0.5,  0.5,  0.5, 1);
         Vector4 v8( 0.5,  0.5,  0.5, 1);
         
         v1 = pnm->comMatrix * v1;
         v2 = pnm->comMatrix * v2;
         v3 = pnm->comMatrix * v3;
         v4 = pnm->comMatrix * v4;
         v5 = pnm->comMatrix * v5;
         v6 = pnm->comMatrix * v6;
         v7 = pnm->comMatrix * v7;
         v8 = pnm->comMatrix * v8;

         calculateAABB(v1, pnm->xRange, pnm->yRange, pnm->zRange);
         calculateAABB(v2, pnm->xRange, pnm->yRange, pnm->zRange);
         calculateAABB(v3, pnm->xRange, pnm->yRange, pnm->zRange);
         calculateAABB(v4, pnm->xRange, pnm->yRange, pnm->zRange);
         calculateAABB(v5, pnm->xRange, pnm->yRange, pnm->zRange);
         calculateAABB(v6, pnm->xRange, pnm->yRange, pnm->zRange);
         calculateAABB(v7, pnm->xRange, pnm->yRange, pnm->zRange);
         calculateAABB(v8, pnm->xRange, pnm->yRange, pnm->zRange);

         //for the root node
         if(m_root->xRange.x > pnm->xRange.x)
             m_root->xRange.x = pnm->xRange.x;
         if(m_root->xRange.y < pnm->xRange.y)
             m_root->xRange.y = pnm->xRange.y;

         if(m_root->yRange.x > pnm->yRange.x)
             m_root->yRange.x = pnm->yRange.x;
         if(m_root->yRange.y < pnm->yRange.y)
             m_root->yRange.y = pnm->yRange.y;

         if(m_root->zRange.x > pnm->zRange.x)
             m_root->zRange.x = pnm->zRange.x;
         if(m_root->zRange.y < pnm->zRange.y)
             m_root->zRange.y = pnm->zRange.y;

     }

     m_root->isLeaf = 0;
     m_root->splitNode();
 }

 void RayScene::calculateAABB(Vector4 v, Vector2& xRange, Vector2& yRange, Vector2& zRange)
 {
     if(v.x > xRange.y)
         xRange.y = v.x;
     if(v.x < xRange.x)
         xRange.x = v.x;

     if(v.y > yRange.y)
         yRange.y = v.y;
     if(v.y < yRange.x)
         yRange.x = v.y;

     if(v.z > zRange.y)
         zRange.y = v.z;
     if(v.z < zRange.x)
         zRange.x = v.z;
 }

 void RayScene::traverseTree(KdtreeNode* node, REAL &t, const Vector4 &p, const Vector4 &d, Vector3& normal, CS123SceneMaterial& tmaterial, Vector2 &textureCo, int &intersectId)
 {
     if(node->isLeaf && node->m_tbd.size() > 0)
     {
         calculateIntersection(t, node->m_tbd, p,d,normal,tmaterial,textureCo, intersectId);
         return ;
     }
     Vector3 n1 = Vector3(0, 1, 0);
     Vector3 n2 = Vector3(1, 0, 0);
     Vector3 n3 = Vector3(0, 0, 1);

     Vector3 p1 = Vector3(node->xRange.y, node->yRange.y, node->zRange.y);
     Vector3 p2 = Vector3(node->xRange.x, node->yRange.x, node->zRange.x);

     int tmpt;
     tmpt = node->calculatePlaneHit(n1, p1, p, d);
     if(node->isInBoundingBox(tmpt, p, d,'y'))
     {
         traverseTree(node->leftChild, t, p, d,normal,tmaterial,textureCo, intersectId);
         traverseTree(node->rightChild,t, p, d,normal,tmaterial,textureCo, intersectId);
         return;
     }

     tmpt = node->calculatePlaneHit(n1, p2, p, d);
     if(node->isInBoundingBox(tmpt, p, d,'y'))
     {
         traverseTree(node->leftChild, t, p, d,normal,tmaterial,textureCo, intersectId);
         traverseTree(node->rightChild, t, p, d,normal,tmaterial,textureCo, intersectId);
         return;
     }

     tmpt = node->calculatePlaneHit(n2, p1, p, d);
     if(node->isInBoundingBox(tmpt, p, d,'x'))
     {
         traverseTree(node->leftChild, t, p, d,normal,tmaterial,textureCo, intersectId);
         traverseTree(node->rightChild,t, p, d,normal,tmaterial,textureCo, intersectId);
         return;
     }

     tmpt = node->calculatePlaneHit(n2, p2, p, d);
     if(node->isInBoundingBox(tmpt, p, d,'x'))
     {
         traverseTree(node->leftChild, t, p, d,normal,tmaterial,textureCo, intersectId);
         traverseTree(node->rightChild,t, p, d,normal,tmaterial,textureCo, intersectId);
         return;
     }

     tmpt = node->calculatePlaneHit(n3, p1, p, d);
     if(node->isInBoundingBox(tmpt, p, d,'z'))
     {
         traverseTree(node->leftChild, t, p, d,normal,tmaterial,textureCo, intersectId);
         traverseTree(node->rightChild,t, p, d,normal,tmaterial,textureCo, intersectId);
         return;
     }

     tmpt = node->calculatePlaneHit(n3, p2, p, d);
     if(node->isInBoundingBox(tmpt, p, d,'z'))
     {
         traverseTree(node->leftChild, t, p, d,normal,tmaterial,textureCo, intersectId);
         traverseTree(node->rightChild,t, p, d,normal,tmaterial,textureCo, intersectId);
         return;
     }
 }

 void KdtreeNode::splitNode()
 {
     if(isLeaf)
         return;

     char splitAxis = 'y';
     int tmp;
     REAL xr = xRange.y - xRange.x;
     REAL yr = yRange.y - yRange.x;
     REAL zr = zRange.y - zRange.x;
     if( xr > yr)
     {
         splitAxis = 'x';
         tmp = xr;
     }
     if( zr > tmp)
     {
         splitAxis = 'z';
     }

     std::vector<primitiveNmatrix*>::iterator it;
     int countl, countr;
     REAL costSum = INT_MAX, thresholdt, costt, threshold;
     switch(splitAxis)
     {
     case 'x':
         for(it = m_tbd.begin(); it != m_tbd.end(); it++)
         {
             primitiveNmatrix* pnm = (*it);
             thresholdt = pnm->xRange.y;
             countChild('x',thresholdt,countl,countr);
             costt = countr * (yr*zr + zr* (xRange.y - thresholdt) + yr* (xRange.y - thresholdt))
                     + countl * (yr*zr + zr* (thresholdt - xRange.x) + yr* (thresholdt - xRange.x));
             if(costSum > costt)
             {
                 threshold = thresholdt;
                 costSum = costt;
             }
         }
         break;
     case 'y':
         for(it = m_tbd.begin(); it != m_tbd.end(); it++)
         {
             primitiveNmatrix* pnm = (*it);
             thresholdt = pnm->yRange.y;
             countChild('y',thresholdt,countl,countr);
             costt = countr * (xr*zr + zr* (yRange.y - thresholdt) + xr* (yRange.y - thresholdt))
                     + countl * (xr*zr + zr* (thresholdt - yRange.x) + xr* (thresholdt - yRange.x));
             if(costSum > costt)
             {
                 threshold = thresholdt;
                 costSum = costt;
             }
         }
         break;
     case 'z':
         for(it = m_tbd.begin(); it != m_tbd.end(); it++)
         {
             primitiveNmatrix* pnm = (*it);
             thresholdt = pnm->zRange.y;
             countChild('z',thresholdt,countl,countr);
             costt = countr * (xr*yr + yr* (zRange.y - thresholdt) + xr* (zRange.y - thresholdt))
                     + countl * (xr*yr + yr* (thresholdt - zRange.x) + xr* (thresholdt - zRange.x));
             if(costSum > costt)
             {
                 threshold = thresholdt;
                 costSum = costt;
             }
         }
         break;
     default:
         break;
     }
     leftChild = new KdtreeNode();
     rightChild = new KdtreeNode();

     addPrimitive2Node(splitAxis, threshold, leftChild, rightChild);

     leftChild->splitNode();
     rightChild->splitNode();
 }

 void KdtreeNode::countChild(char axis, REAL threshold, int& left, int & right)
 {
     std::vector<primitiveNmatrix*>::iterator it;
     left = 0;
     right = 0;
     for(it = m_tbd.begin(); it != m_tbd.end(); it++)
     {
         primitiveNmatrix* pnm = (*it);
         switch(axis)
         {
         case 'x':
             if(pnm->xRange.x < threshold)
                 left ++;
             if(pnm->xRange.y > threshold)
                 right ++;
             break;
         case 'y':
             if(pnm->yRange.x < threshold)
                 left ++;
             if(pnm->yRange.y > threshold)
                 right ++;
             break;
         case 'z':
             if(pnm->zRange.x < threshold)
                 left ++;
             if(pnm->zRange.y > threshold)
                 right ++;
             break;
         default:
             break;
         }
     }
 }

 void KdtreeNode::addPrimitive2Node(char axis, REAL threshold, KdtreeNode *left, KdtreeNode *right)
 {
     std::vector<primitiveNmatrix*>::iterator it;

     switch(axis)
     {
     case 'x':
         left->yRange = yRange;
         left->zRange = zRange;

         right->yRange = yRange;
         right->zRange = zRange;

         left->xRange.x = xRange.x;
         left->xRange.y = threshold;

         right->xRange.x = threshold;
         right->xRange.y = xRange.y;
         for(it = m_tbd.begin(); it != m_tbd.end(); it++)
         {
             primitiveNmatrix* pnm = (*it);
             if(pnm->xRange.x < threshold)
                 left->m_tbd.push_back(pnm);
             if(pnm->xRange.y > threshold)
                 right->m_tbd.push_back(pnm);
         }
         break;
     case 'y':
         left->xRange = xRange;
         left->zRange = zRange;

         right->xRange = xRange;
         right->zRange = zRange;

         left->yRange.x = yRange.x;
         left->yRange.y = threshold;

         right->yRange.x = threshold;
         right->yRange.y = yRange.y;
         for(it = m_tbd.begin(); it != m_tbd.end(); it++)
         {
             primitiveNmatrix* pnm = (*it);

             if(pnm->yRange.x < threshold)
                 left->m_tbd.push_back(pnm);
             if(pnm->yRange.y > threshold)
                 right->m_tbd.push_back(pnm);
         }
         break;
     case 'z':
         left->xRange = xRange;
         left->yRange = yRange;

         right->xRange = xRange;
         right->yRange = yRange;

         left->zRange.x = zRange.x;
         left->zRange.y = threshold;

         right->zRange.x = threshold;
         right->zRange.y = zRange.y;
         for(it = m_tbd.begin(); it != m_tbd.end(); it++)
         {
             primitiveNmatrix* pnm = (*it);
             if(pnm->zRange.x < threshold)
                 left->m_tbd.push_back(pnm);
             if(pnm->zRange.y > threshold)
                 right->m_tbd.push_back(pnm);
         }
         break;
     default:
         break;
     }

     if(left->m_tbd.size() < 5)
         left->isLeaf = true;
     else
         left->isLeaf = false;

     if(right->m_tbd.size() < 5)
         right->isLeaf = true;
     else
         right->isLeaf = false;
 }


 REAL KdtreeNode::calculatePlaneHit(const Vector3& n, const Vector3& p0, const Vector4 &p, const Vector4 &d)
 {
     return ((n.x*p0.x + n.y*p0.y+n.z*p0.z) - (n.x*p.x + n.y*p.y + n.z*p.z))/(n.x*d.x + n.y*d.y + n.z*d.z);
 }

 bool KdtreeNode::isInBoundingBox(const REAL& tmpt, const Vector4 &p, const Vector4 &d, char signal)
 {
     if(tmpt >=0 )
     {
         REAL x = p.x + tmpt*d.x;
         REAL y = p.y + tmpt*d.y;
         REAL z = p.z + tmpt*d.z;
         switch(signal)
         {
         case 'x':
             if(y < yRange.x || y > yRange.y || z < zRange.x || z > zRange.y)
                 return false;
             else
                 return true;
             break;
         case 'y':
             if(x < xRange.x || x > xRange.y || z < zRange.x || z > zRange.y)
                 return false;
             else
                 return true;
             break;
         case 'z':
             if(x < xRange.x || x > xRange.y || y < yRange.x || y > yRange.y )
                 return false;
             else
                 return true;
             break;
         default:
             break;
         }
     }
     else
         return false;
 }

 void RayScene::clear( KdtreeNode * node)
 {
     if(node->isLeaf)
         delete node;
     else
     {
         clear(node->leftChild);
         clear(node->rightChild);
     }
 }
// int RayScene::xsort_helper(const void* a, const void* b)
// {
//     primitiveNmatrix* arg1 = *reinterpret_cast<const primitiveNmatrix*>(a);
//     primitiveNmatrix* arg2 = *reinterpret_cast<const primitiveNmatrix*>(b);
//     if(arg1->xRange.x < arg2->xRange.x) return -1;
//     if(arg1->xRange.x > arg2->xRange.x) return 1;
//     return 0;
// }

// int RayScene::ysort_helper(const void* a, const void* b)
// {
//     primitiveNmatrix* arg1 = *reinterpret_cast<const primitiveNmatrix*>(a);
//     primitiveNmatrix* arg2 = *reinterpret_cast<const primitiveNmatrix*>(b);
//     if(arg1->yRange.x < arg2->yRange.x) return -1;
//     if(arg1->yRange.x > arg2->yRange.x) return 1;
//     return 0;
// }

// int RayScene::zsort_helper(const void* a, const void* b)
// {
//     primitiveNmatrix* arg1 = *reinterpret_cast<const primitiveNmatrix*>(a);
//     primitiveNmatrix* arg2 = *reinterpret_cast<const primitiveNmatrix*>(b);
//     if(arg1->zRange.x < arg2->zRange.x) return -1;
//     if(arg1->zRange.x > arg2->zRange.x) return 1;
//     return 0;
// }
