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
         primitiveNmatrix pnm = (*it);
         m_root->m_tbdId.push_back(pnm.id);

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
         if(m_root->xRange.x > pnm.xRange.x)
             m_root->xRange.x = pnm.xRange.x;
         if(m_root->xRange.y < pnm.xRange.y)
             m_root->xRange.y = pnm.xRange.y;

         if(m_root->yRange.x > pnm.yRange.x)
             m_root->yRange.x = pnm.yRange.x;
         if(m_root->yRange.y < pnm.yRange.y)
             m_root->yRange.y = pnm.yRange.y;

         if(m_root->zRange.x > pnm.zRange.x)
             m_root->zRange.x = pnm.zRange.x;
         if(m_root->zRange.y < pnm.zRange.y)
             m_root->zRange.y = pnm.zRange.y;

         pnmDict[pnm.id] = pnm;

     }

     m_root->isLeaf = 0;
     splitNode(m_root, 0);
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

 void RayScene::splitNode(KdtreeNode* node, int depth)
 {
     if(node->isLeaf)
     {
         return;
     }

     char splitAxis = 'y';
     int tmp;
     REAL xr = node->xRange.y - node->xRange.x;
     REAL yr = node->yRange.y - node->yRange.x;
     REAL zr = node->zRange.y - node->zRange.x;
     if( xr > yr)
     {
         splitAxis = 'x';
         tmp = xr;
     }
     if( zr > tmp)
     {
         splitAxis = 'z';
     }

     std::vector<int>::iterator it;
     int countl, countr;
     REAL costSum = INT_MAX, thresholdt, costt, threshold;
     switch(splitAxis)
     {
     case 'x':
         for(it = node->m_tbdId.begin(); it != node->m_tbdId.end(); it++)
         {
             int Id = (*it);
             thresholdt = pnmDict[Id].xRange.y;
             countChild(node, 'x',thresholdt,countl,countr);
             costt = countr * (yr*zr + zr* (node->xRange.y - thresholdt) + yr* (node->xRange.y - thresholdt))
                     + countl * (yr*zr + zr* (thresholdt - node->xRange.x) + yr* (thresholdt - node->xRange.x));
             if(costSum > costt)
             {
                 threshold = thresholdt;
                 costSum = costt;
             }
         }
         break;
     case 'y':
         for(it = node->m_tbdId.begin(); it != node->m_tbdId.end(); it++)
         {
             int Id = (*it);
             thresholdt = pnmDict[Id].yRange.y;
             countChild(node, 'y',thresholdt,countl,countr);
             costt = countr * (xr*zr + zr* (node->yRange.y - thresholdt) + xr* (node->yRange.y - thresholdt))
                     + countl * (xr*zr + zr* (thresholdt - node->yRange.x) + xr* (thresholdt - node->yRange.x));
             if(costSum > costt)
             {
                 threshold = thresholdt;
                 costSum = costt;
             }
         }
         break;
     case 'z':
         for(it = node->m_tbdId.begin(); it != node->m_tbdId.end(); it++)
         {
             int Id = (*it);
             thresholdt = pnmDict[Id].xRange.y;
             countChild(node, 'z',thresholdt,countl,countr);
             costt = countr * (xr*yr + yr* (node->zRange.y - thresholdt) + xr* (node->zRange.y - thresholdt))
                     + countl * (xr*yr + yr* (thresholdt - node->zRange.x) + xr* (thresholdt - node->zRange.x));
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
     node->leftChild = new KdtreeNode();
     node->rightChild = new KdtreeNode();

     addPrimitive2Node(node,splitAxis, threshold);

     splitNode(node->leftChild, depth+1);
     splitNode(node->rightChild, depth+1);
 }

 void RayScene::countChild(KdtreeNode *node, char axis, REAL threshold, int& left, int & right)
 {
     std::vector<int>::iterator it;
     left = 0;
     right = 0;
     for(it = node->m_tbdId.begin(); it != node->m_tbdId.end(); it++)
     {
         int Id = (*it);
         switch(axis)
         {
         case 'x':
             if(pnmDict[Id].xRange.x < threshold)
                 left ++;
             if(pnmDict[Id].xRange.y > threshold)
                 right ++;
             break;
         case 'y':
             if(pnmDict[Id].yRange.x < threshold)
                 left ++;
             if(pnmDict[Id].yRange.y > threshold)
                 right ++;
             break;
         case 'z':
             if(pnmDict[Id].zRange.x < threshold)
                 left ++;
             if(pnmDict[Id].zRange.y > threshold)
                 right ++;
             break;
         default:
             break;
         }
     }
 }

 void RayScene::addPrimitive2Node(KdtreeNode *node, char axis, REAL threshold)
 {
     std::vector<int>::iterator it;

     switch(axis)
     {
     case 'x':
         node->leftChild->yRange = node->yRange;
         node->leftChild->zRange = node->zRange;

         node->rightChild->yRange = node->yRange;
         node->rightChild->zRange = node->zRange;

         node->leftChild->xRange.x = node->xRange.x;
         node->leftChild->xRange.y = threshold;

         node->rightChild->xRange.x = threshold;
         node->rightChild->xRange.y = node->xRange.y;
         for(it = node->m_tbdId.begin(); it != node->m_tbdId.end(); it++)
         {
             int Id = (*it);
             if(pnmDict[Id].xRange.x < threshold)
                 node->leftChild->m_tbdId.push_back(Id);
             if(pnmDict[Id].xRange.y > threshold)
                 node->rightChild->m_tbdId.push_back(Id);
         }
         break;
     case 'y':
         node->leftChild->xRange = node->xRange;
         node->leftChild->zRange = node->zRange;

         node->rightChild->xRange = node->xRange;
         node->rightChild->zRange = node->zRange;

         node->leftChild->yRange.x = node->yRange.x;
         node->leftChild->yRange.y = threshold;

         node->rightChild->yRange.x = threshold;
         node->rightChild->yRange.y = node->yRange.y;
         for(it = node->m_tbdId.begin(); it != node->m_tbdId.end(); it++)
         {
             int Id = (*it);
             if(pnmDict[Id].yRange.x < threshold)
                 node->leftChild->m_tbdId.push_back(Id);
             if(pnmDict[Id].yRange.y > threshold)
                 node->rightChild->m_tbdId.push_back(Id);
         }
         break;
     case 'z':
         node->leftChild->xRange = node->xRange;
         node->leftChild->yRange = node->yRange;

         node->rightChild->xRange = node->xRange;
         node->rightChild->yRange = node->yRange;

         node->leftChild->zRange.x = node->zRange.x;
         node->leftChild->zRange.y = threshold;

         node->rightChild->zRange.x = threshold;
         node->rightChild->zRange.y = node->zRange.y;
         for(it = node->m_tbdId.begin(); it != node->m_tbdId.end(); it++)
         {
             int Id = (*it);
             if(pnmDict[Id].zRange.x < threshold)
                 node->leftChild->m_tbdId.push_back(Id);
             if(pnmDict[Id].zRange.y > threshold)
                 node->rightChild->m_tbdId.push_back(Id);
         }
         break;
     default:
         break;
     }

     if(node->leftChild->m_tbdId.size() < 6)
         node->leftChild->isLeaf = true;
     else
         node->leftChild->isLeaf = false;

     if(node->rightChild->m_tbdId.size() < 6)
         node->rightChild->isLeaf = true;
     else
         node->rightChild->isLeaf = false;
 }
