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
     m_root->splitNode(0);
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


