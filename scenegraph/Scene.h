#ifndef SCENE_H
#define SCENE_H

#include <CS123Common.h>
#include "CS123SceneData.h"

class Camera;
class CS123ISceneParser;


/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */

struct primitiveNmatrix
{
    int id;
    PrimitiveType shapetype;
    CS123SceneMaterial material;
    glm::mat4 comMatrix;

    GLuint m_vaoID; // The vertex array object ID, which is passed to glBindVertexArray.
    int verticesNumber;
    
    Vector2 xRange, yRange, zRange;
};

class Scene
{
public:
    Scene();
    virtual ~Scene();

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

    std::vector<CS123SceneLightData> getSceneLight() {return m_scenelights;}
    CS123SceneGlobalData getSceneGlobalData()   {return m_sceneglobalData;}
    std::vector<primitiveNmatrix> getPrimitiveList() {return m_tbd;}

    void setSceneLight(std::vector<CS123SceneLightData> sl)  {    m_scenelights = sl;}
    void setPrimitiveList(std::vector<primitiveNmatrix> pl)  {    m_tbd = pl;}
    void setGlobal(const CS123SceneGlobalData &global) {      m_sceneglobalData = global; }

    int counter;

protected:

    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.

    void analyzeNode(CS123SceneNode* rnode, glm::mat4 CTM);

    std::vector<CS123SceneLightData> m_scenelights;
    CS123SceneGlobalData m_sceneglobalData;
    std::vector<primitiveNmatrix> m_tbd;

    glm::mat4x4 getRotMat( const Vector3 &v, const REAL a) {
        REAL theta, phi;
        theta = atan2(v.z, v.x);
        phi = -atan2(v.y, sqrt(v.x*v.x + v.z*v.z));

        glm::mat4x4 M1 = glm::transpose(glm::mat4x4(cos(theta), 0, sin(theta), 0,
                                                    0, 1, 0, 0,
                                                    -sin(theta), 0, cos(theta), 0,
                                                    0, 0, 0, 1));
        glm::mat4x4 M2 = glm::transpose(glm::mat4x4(cos(phi), -sin(phi), 0, 0,
                                                    sin(phi), cos(phi), 0, 0,
                                                    0, 0, 1, 0,
                                                    0, 0, 0, 1));
        glm::mat4x4 M3 = glm::transpose(glm::mat4x4(1, 0, 0, 0,
                                                    0, cos(a), -sin(a), 0,
                                                    0, sin(a), cos(a), 0,
                                                    0, 0, 0, 1));

        glm::mat4x4 invM1 = glm::transpose(glm::mat4x4(cos(theta), 0, -sin(theta), 0,
                                                       0, 1, 0, 0,
                                                       sin(theta), 0, cos(theta), 0,
                                                       0, 0, 0, 1));
        glm::mat4x4 invM2 = glm::transpose(glm::mat4x4(cos(phi), sin(phi), 0, 0,
                                                       -sin(phi), cos(phi), 0, 0,
                                                       0, 0, 1, 0,
                                                       0, 0, 0, 1));


        glm::mat4x4 M = invM1 * invM2 * M3 * M2* M1;

        return M;
    }

    glm::mat4 getScaleMat(const Vector3 &v) {
        glm::mat4x4 scale = glm::transpose(glm::mat4x4(v.x, 0, 0, 0,
                                                       0, v.y, 0, 0,
                                                       0, 0, v.z, 0,
                                                       0, 0, 0, 1));
        return scale;
    }

    glm::mat4 getTransMat(const Vector3 &v) {
        glm::mat4x4  trans = glm::transpose(glm::mat4x4(1, 0, 0, v.x,
                                                        0, 1, 0, v.y,
                                                        0, 0, 1, v.z,
                                                        0, 0, 0, 1));
        return trans;
    }

};

#endif // SCENE_H


