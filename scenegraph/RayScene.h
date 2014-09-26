#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "shape/Shape.h"
#include "shape/Cube.h"
#include "shape/Cone.h"
#include "shape/Sphere.h"
#include "shape/Cylinder.h"
#include "shape/Torus.h"
#include "shape/Fractal.h"

/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene
{
public:
    RayScene();
    virtual ~RayScene();

};

#endif // RAYSCENE_H
