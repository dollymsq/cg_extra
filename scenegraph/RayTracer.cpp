#include "RayTracer.h"

RayTracer::RayTracer()
{
}

void RayTracer::prepare(Vector4 e, int w, int h, glm::mat4 M, BGRA *d, RayScene* m_s, int odr, int interval)
{
    eye = e;
    width = w;
    height = h;
    Mc2w = M;
    data = d;
    m_root = m_s->m_root;
    texImgPair = m_s->texImgPair;
    setGlobal(m_s->getSceneGlobalData());
    setSceneLight(m_s->getSceneLight());
    pnmDict = m_s->pnmDict;

    m_order = odr;
    m_interval = interval;
}

Vector4 RayTracer::generateRay(Vector4 eye, Vector4 filmP, glm::mat4 M)
{
    Vector4 worldP = M * filmP;
    Vector4 tp = worldP - eye;
    Vector4 dir= glm::normalize(tp);

    return dir;
}

CS123SceneColor RayTracer::trace(Vector4 eye, Vector4 dir)
{
    int intersectId = -1;
    m_intersectId = -1;

    CS123SceneColor flcolor= rayTrace(eye, dir, 0, intersectId);
    return flcolor;
}

CS123SceneColor RayTracer::rayTrace(Vector4 eye, Vector4 dir, int recursiondepth, int intersectId)
{
    Vector3 tnormal(0, 0, 0); // normal at point t
    CS123SceneMaterial tmaterial;
    Vector2 tex(0,0);

//    std::cout<<recursiondepth<<"---------------------"<<endl;
//    std::cout<<intersectId<<endl;

    REAL t = MAX_LIMIT;

    traverseTree(m_root,t, eye,dir,tnormal,tmaterial, tex, intersectId);

//    std::cout<<intersectId<<endl;
//    std::cout<<t<<endl;

    CS123SceneColor flcolor;
    flcolor = flcolor * 0;

    if(t < MAX_LIMIT && recursiondepth < RECURSION_LIMIT)
    {
        flcolor = illuminatePoint(eye, dir, t, tnormal, tmaterial,tex,intersectId,recursiondepth);
    }
    flcolor = flcolor.threshold();
    return flcolor;
}

Vector3 RayTracer::transNormalo2w(Vector3 normal,glm::mat4 M)
{
    Vector3 tp;

    tp.x = M[0][0]* normal.x + M[0][1]* normal.y + M[0][2]* normal.z;
    tp.y = M[1][0]* normal.x + M[1][1]* normal.y + M[1][2]* normal.z;
    tp.z = M[2][0]* normal.x + M[2][1]* normal.y + M[2][2]* normal.z;

    return glm::normalize(tp);
}

CS123SceneColor RayTracer::illuminatePoint(Vector4 eye, Vector4 dir, REAL t, Vector3 n, CS123SceneMaterial matrl, Vector2 textureCo, int intersectId, int recursiondepth)
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
    Vector3 viewray(dir.x, dir.y, dir.z);
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
            Vector3 coeff2 = specularShade(lm,n,-viewray,matrl);
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
            Vector3 coeff2 = specularShade(lightdirInverse,n,-viewray,matrl);
            tcolor.b += (*it).color.b * coeff2.x;
            tcolor.g += (*it).color.g * coeff2.y;
            tcolor.r += (*it).color.r * coeff2.z;
        }
    }

    //reflected part
    if(settings.useReflection && matrl.cReflective.b != 0 && matrl.cReflective.g != 0 && matrl.cReflective.r != 0)
    {
        Vector3 rflvr = glm::reflect(viewray, n);
        Vector4 rflvrv4 = glm::normalize(Vector4(rflvr.x, rflvr.y, rflvr.z, 0));
        m_intersectId = intersectId;
        CS123SceneColor nextPointColor = rayTrace(pos_objv4,rflvrv4,recursiondepth + 1, intersectId);

        tcolor.b += m_sceneglobalData.ks * matrl.cReflective.b * nextPointColor.b;
        tcolor.g += m_sceneglobalData.ks * matrl.cReflective.g * nextPointColor.g;
        tcolor.r += m_sceneglobalData.ks * matrl.cReflective.r * nextPointColor.r;
    }

    //refracted part
    if(settings.useRefraction && matrl.cTransparent.b != 0 && matrl.cTransparent.g != 0 && matrl.cTransparent.r != 0)
    {
        Vector3 rfrvr;
        if(glm::dot(viewray, n) < 0)
            rfrvr = glm::refract(viewray, n, matrl.ior);
        else
            rfrvr = glm::refract(viewray, -n, 1.0f/matrl.ior);

//        rfrvr = viewray;
        pos_objv4 = pos_objv4 + dir * 0.00001f ;
        Vector4 rfrvrv4 = glm::normalize(Vector4(rfrvr.x, rfrvr.y, rfrvr.z, 0));
        m_intersectId = intersectId;
        CS123SceneColor nextPointColor = rayTrace(pos_objv4,rfrvrv4,recursiondepth, intersectId);

        tcolor.b += m_sceneglobalData.kt * matrl.cTransparent.b * nextPointColor.b;
        tcolor.g += m_sceneglobalData.kt * matrl.cTransparent.g * nextPointColor.g;
        tcolor.r += m_sceneglobalData.kt * matrl.cTransparent.r * nextPointColor.r;

//        tcolor.b = 0.8 * tcolor.b +  m_sceneglobalData.kt * matrl.cTransparent.b * nextPointColor.b;
//        tcolor.g = 0.8 * tcolor.g +  m_sceneglobalData.kt * matrl.cTransparent.g * nextPointColor.g;
//        tcolor.r = 0.8 * tcolor.r +  m_sceneglobalData.kt * matrl.cTransparent.r * nextPointColor.r;
    }


    return tcolor;
}

bool RayTracer::isInShadow(Vector3 object, Vector3 raydir, REAL tmin, int intersectId)
{
    Vector4 objectv4(object.x, object.y, object.z, 1);
    Vector4 shadowray(raydir.x, raydir.y, raydir.z, 0);

    Vector3 tnormal(0, 0, 0); // normal at point t
    CS123SceneMaterial tmaterial;
    Vector2 tex(0,0);
    REAL t = MAX_LIMIT;
    m_intersectId = intersectId;
    traverseTree(m_root, t, objectv4, shadowray,tnormal,tmaterial,tex, intersectId);
    if( t < tmin)
        return true;
    else
        return false;
}

void RayTracer::calculateIntersection(REAL &tpostmin, std::vector<int> node_tbdId, Vector4 start, Vector4 dir, Vector3& normal, CS123SceneMaterial& tmaterial, Vector2 &textureCo, int &intersectId)
{
    Shape* m_vshape = NULL;
    glm::mat4 Mw2o;
    std::vector<int>::iterator it;
    Vector3 tnormal(0,0,0);
    int tpId = intersectId;
    REAL t = MAX_LIMIT; // to compare
    Vector2 textureCot = Vector2(0,0);

    for(it = node_tbdId.begin(); it != node_tbdId.end(); it++)
    {
        int Id = (*it);
        if(pnmDict[Id].shapetype!= PRIMITIVE_CUBE && pnmDict[Id].shapetype!= PRIMITIVE_CONE && pnmDict[Id].shapetype!= PRIMITIVE_CYLINDER && pnmDict[Id].shapetype!= PRIMITIVE_SPHERE)
            continue;
        if( Id == m_intersectId)
            continue;
        Mw2o = glm::inverse(pnmDict[Id].comMatrix);
        Vector4 p = Mw2o*start;
        Vector4 d = Mw2o*dir;

        switch(pnmDict[Id].shapetype)
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
            t = m_vshape->calculateIntersecP(tnormal,textureCot);
            delete m_vshape;
            m_vshape = NULL;
        }

        if(t>1E-6 && t < tpostmin)
        {
            tpostmin = t;
            normal = transNormalo2w(tnormal,glm::inverse(pnmDict[Id].comMatrix));
            tmaterial = pnmDict[Id].material;
            textureCo = textureCot;
            tpId = Id;
        }
    }
    intersectId = tpId;
}

Vector3 RayTracer::diffuseShade(Vector3 lightray, Vector3 n, CS123SceneMaterial matrl, Vector2 textureCo)
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

Vector3 RayTracer::specularShade(Vector3 lightray, Vector3 n, Vector3 viewray, CS123SceneMaterial matrl)
{
    Vector3 coeff(0,0,0);

    //specular part
    Vector3 reflectedray = - glm::reflect(lightray, n);
    reflectedray = glm::normalize(reflectedray);
//    REAL spedotproduct = min(glm::dot(reflectedray, viewray), 0.6f);
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

CS123SceneColor RayTracer::textureShade(CS123SceneMaterial matrl, Vector2 textureCo, CS123SceneColor tcolor)
{
    CS123SceneColor mcl;
    QRgb c1, c2, c3, c4;
    int idxs1, idxt1 , idxs2, idxt2, idxs3, idxt3, idxs4, idxt4;
    float w1, w2, w3, w4;
    if(matrl.textureMap->isUsed)
    {
        QImage tp = texImgPair[matrl.textureMap->filename];

        float idxs, idxt;
        idxs = (textureCo.x*matrl.textureMap->repeatU - floor(textureCo.x*matrl.textureMap->repeatU))*tp.width();
        idxt = (textureCo.y*matrl.textureMap->repeatV - floor(textureCo.y*matrl.textureMap->repeatV))*tp.height();

        idxs1 = floor(idxs);        idxt1 = floor(idxt);
        idxs2 = floor(idxs);        idxt2 = ceil(idxt);
        idxs3 = ceil(idxs);        idxt3 = ceil(idxt);
        idxs4 = ceil(idxs);        idxt4 = floor(idxt);

        idxs1 = handleEdgeIndex(idxs1, tp.width());
        idxs2 = handleEdgeIndex(idxs2, tp.width());
        idxs3 = handleEdgeIndex(idxs3, tp.width());
        idxs4 = handleEdgeIndex(idxs4, tp.width());

        idxt1 = handleEdgeIndex(idxt1, tp.height());
        idxt2 = handleEdgeIndex(idxt2, tp.height());
        idxt3 =  handleEdgeIndex(idxt3, tp.height());
        idxt4 =  handleEdgeIndex(idxt4, tp.height());

        w1 = fabs(idxs - idxs1) + fabs(idxt - idxt1);
        w2 = fabs(idxs - idxs2) + fabs(idxt - idxt2);
        w3 = fabs(idxs - idxs3) + fabs(idxt - idxt3);
        w4 = fabs(idxs - idxs4) + fabs(idxt - idxt4);

        c1 = tp.pixel(idxs1, idxt1);
        c2 = tp.pixel(idxs2, idxt2);
        c3 = tp.pixel(idxs3, idxt3);
        c4 = tp.pixel(idxs4, idxt4);

        mcl.b = (float(qBlue(c1))/255.0f + float(qBlue(c2))/255.0f
                + float(qBlue(c3))/255.0f + float(qBlue(c4))/255.0f) / 4.0f;
        mcl.g = (float(qGreen(c1))/255.0f + float(qGreen(c2))/255.0f
                + float(qGreen(c3))/255.0f + float(qGreen(c4))/255.0f) / 4.0f;
        mcl.r = (float(qRed(c1))/255.0f + float(qRed(c2))/255.0f
                 + float(qRed(c3))/255.0f + float(qRed(c4))/255.0f) / 4.0f;

        mcl.b = (1-matrl.blend)*tcolor.b + matrl.blend * mcl.b;
        mcl.g = (1-matrl.blend)*tcolor.g + matrl.blend * mcl.g;
        mcl.r = (1-matrl.blend)*tcolor.r + matrl.blend * mcl.r;

        return mcl;
    }
    else
        return tcolor;
}

void RayTracer::traverseTree(KdtreeNode* node, REAL &t, const Vector4 &p, const Vector4 &d, Vector3& normal, CS123SceneMaterial& tmaterial, Vector2 &textureCo, int &intersectId)
{
    if(node->isLeaf)
    {
        if(node->m_tbdId.size() > 0)
            calculateIntersection(t, node->m_tbdId, p,d,normal,tmaterial,textureCo, intersectId);
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

void RayTracer::run()
{
    std::cout << m_order + 1 << " running"<<endl;
    if(settings.useSuperSampling && settings.numSuperSamples >= 2)
    {
        for(int i = m_order; i< height; i = i + m_interval)
        {
            for(int j = 0; j< width; j++)
            {
//                if(i == 176 && j == 206)
//                {
//                    std::cout<<"Image pos"<< j<<", "<<i<<"========================="<<endl;
//                }
                CS123SceneColor flcolor= superSample(eye, Mc2w, j-0.5, j+0.5, i-0.5, i+0.5, 0);
                BGRA canvascolor;
                canvascolor.b = round(255.0 * flcolor.b);
                canvascolor.g = round(255.0 * flcolor.g);
                canvascolor.r = round(255.0 * flcolor.r);

                data[i*width + j] = canvascolor;
           }
        }
    }
    else
    {
        Vector4 filmP(0,0,-1,1);
        for(int i = m_order; i< height; i = i + m_interval)
        {
            for(int j = 0; j< width; j++)
            {
                //screen to film done(camera)
                filmP.x = 2.0*(float)j/(float)width-1.0;
                filmP.y = 1.0-2.0*(float)i/(float)height;

                //to generate ray in world space, the filmP will corresponds to d, copy that
                Vector4 dir = generateRay(eye, filmP, Mc2w);

                //calculate intersection point if any and its corresponding normal
                //                if(i == 297 && j == 484)
                //                if(i == 214 && j == 123)
                //                std::cout<<"Image pos"<< j<<", "<<i<<"========================="<<endl;

                CS123SceneColor flcolor= trace(eye,dir);
                BGRA canvascolor;
                canvascolor.b = round(255.0 * flcolor.b);
                canvascolor.g = round(255.0 * flcolor.g);
                canvascolor.r = round(255.0 * flcolor.r);

                data[i*width + j] = canvascolor;
            }
        }
    }
    std::cout<< m_order + 1 << " finished"<<endl;
}

CS123SceneColor RayTracer::superSample(const Vector4 &eye, const glm::mat4 &Mc2w, double xmin, double xmax, double ymin, double ymax, int depth)
{
    Vector4 filmP(0,0,-1,1);
    filmP.x = (2.0 * (xmin+xmax)/2.0 / width) - 1.0;
    filmP.y = 1.0 - (2.0 * (ymin+ymax)/2.0 / height);
    Vector4 dir = generateRay(eye, filmP, Mc2w);
    CS123SceneColor colorct = trace(eye,dir);

//    if(colorct.b > 0 || colorct.g > 0 || colorct.r > 0 )
//    {
//        std::cout<<"gotcolor"<<endl;
//    }

    std::vector<CS123SceneColor> colorlist;
    std::vector<Vector4> filmPlist;

    for( int i = 0; i < settings.numSuperSamples - 1; i ++ )
    {
        Vector4 filmPt(0,0,-1,1);
        float tmpx = randBtw(xmin, xmax);
        float tmpy = randBtw(ymin, ymax);
        filmPt.x = 2.0 * tmpx/ width - 1.0;
        filmPt.y = 1.0 - 2.0 * tmpy/ height;
        filmPlist.push_back(filmPt);

        Vector4 tdir = generateRay(eye, filmPt, Mc2w);
        CS123SceneColor color = trace(eye,tdir);
        colorlist.push_back(color);
    }

    CS123SceneColor sum = colorct;
    for( int i = 0; i < settings.numSuperSamples - 1; i ++ )
    {
        if(colorct.dist(colorlist[i]) < 0.2 || depth > 2)
        {}
        else
            colorlist[i] = superSample(eye, Mc2w, min(filmP.x, filmPlist[i].x), max(filmP.x, filmPlist[i].x),
                                min(filmP.y, filmPlist[i].y), max(filmP.y, filmPlist[i].y), depth + 1);
        sum = sum + colorlist[i];
    }
    return sum * (1.0f / settings.numSuperSamples);
}
