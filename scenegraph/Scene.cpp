#include "Scene.h"
#include "Camera.h"
#include "CS123SceneData.h"
#include "CS123ISceneParser.h"
#include <QtGlobal>


Scene::Scene() //: m_camera(NULL)
{
    m_scenelights.clear();
    m_tbd.clear();
}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D

}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser)
{
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()
    CS123SceneNode* rootnode = parser->getRootNode();

    CS123SceneGlobalData gdata;
    CS123SceneLightData ldata;

    parser->getGlobalData(gdata);
    sceneToFill->setGlobal(gdata);

    for(int i = 0; i < parser->getNumLights(); i ++)
    {
        parser->getLightData(i,ldata);
        sceneToFill->addLight(ldata);
    }

    glm::mat4 tM = glm::mat4();


    sceneToFill->analyzeNode(rootnode, tM);
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix)
{
    primitiveNmatrix *t = new primitiveNmatrix;
    t->shapetype = scenePrimitive.type;
    t->comMatrix = matrix;

    CS123SceneMaterial& mat = t->material;

    memcpy(&mat, &scenePrimitive.material, sizeof(CS123SceneMaterial));
    mat.textureMap = new CS123SceneFileMap;
    mat.bumpMap = new CS123SceneFileMap;

    mat.textureMap->filename = scenePrimitive.material.textureMap->filename;
    mat.textureMap->repeatU = scenePrimitive.material.textureMap->repeatU;
    mat.textureMap->repeatV = scenePrimitive.material.textureMap->repeatV;
    mat.textureMap->isUsed = scenePrimitive.material.textureMap->isUsed;

    m_tbd.push_back(*t);
    delete t;
}

void Scene::addLight(const CS123SceneLightData &sceneLight)
{
     m_scenelights.push_back(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global)
{
    m_sceneglobalData = global;
}

void Scene::analyzeNode(CS123SceneNode *rnode, glm::mat4 CTM)
{
    if(!rnode->transformations.empty())
    {
        for(uint j = 0; j < rnode->transformations.size(); j ++)
        {
            CS123SceneTransformation *transf = rnode->transformations[j];
            switch(transf->type)
            {
            case TRANSFORMATION_TRANSLATE:
                CTM *= getTransMat(transf->translate);
                break;
            case  TRANSFORMATION_SCALE:
                CTM *= getScaleMat(transf->scale);
                break;
            case TRANSFORMATION_ROTATE:
                CTM *= getRotMat(transf->rotate, transf->angle);
                break;
            case TRANSFORMATION_MATRIX:
                CTM *= transf->matrix;
                break;
            default:
                break;
            }
        }
    }
    if(!rnode->primitives.empty())
    {
        for(uint j = 0; j < rnode->primitives.size(); j ++)
        {
            addPrimitive(*(rnode->primitives[j]),CTM);
        }
    }
    if(!rnode->children.empty())
    {
        for(uint j = 0; j < rnode->children.size(); j ++)
            analyzeNode(rnode->children[j],CTM);
    }
}

std::vector<CS123SceneLightData> Scene::getSceneLight()
{
    return m_scenelights;
}

CS123SceneGlobalData Scene::getSceneGlobalData()
{
    return m_sceneglobalData;
}

std::vector<primitiveNmatrix> Scene::getPrimitiveList()
{
    return m_tbd;
}

void Scene::setSceneLight(std::vector<CS123SceneLightData> sl)
{
    m_scenelights = sl;
}

void Scene::setPrimitiveList(std::vector<primitiveNmatrix> pl)
{
    m_tbd = pl;
}


