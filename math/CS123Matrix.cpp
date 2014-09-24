/*!
   @file   CS123Matrix.cpp
   @author Travis Fischer (fisch0920@gmail.com)
   @date   Fall 2008
   
   @brief
      Provides basic functionality for a templated, arbitrarily-sized matrix.
      You will need to fill this file in for the Camtrans assignment.

**/

#include "CS123Algebra.h"
#include <iostream>

//@name Routines which generate specific-purpose transformation matrices
//@{---------------------------------------------------------------------
// @returns the scale matrix described by the vector
Matrix4x4 getScaleMat(const Vector4 &v) {


    // @TODO: [CAMTRANS] Fill this in...
    Matrix4x4 scale(v.x, 0, 0, 0,
                    0, v.y, 0, 0,
                    0, 0, v.z, 0,
                    0, 0, 0, 1);
    return scale;
}

// @returns the translation matrix described by the vector
Matrix4x4 getTransMat(const Vector4 &v) {
    // @TODO: [CAMTRANS] Fill this in...
    Matrix4x4 trans(1, 0, 0, v.x,
                    0, 1, 0, v.y,
                    0, 0, 1, v.z,
                    0, 0, 0, 1);
    return trans;

}

// @returns the rotation matrix about the x axis by the specified angle
Matrix4x4 getRotXMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    Matrix4x4 rotx(1, 0, 0, 0,
                   0, cos(radians), -sin(radians), 0,
                   0, sin(radians), cos(radians), 0,
                   0, 0, 0, 1);
    return rotx;

}

// @returns the rotation matrix about the y axis by the specified angle
Matrix4x4 getRotYMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    Matrix4x4 roty(cos(radians), 0, sin(radians), 0,
                   0, 1, 0, 0,
                   -sin(radians), 0, cos(radians), 0,
                   0, 0, 0, 1);
    return roty;

}

// @returns the rotation matrix about the z axis by the specified angle
Matrix4x4 getRotZMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    Matrix4x4 rotz(cos(radians), -sin(radians), 0, 0,
                   sin(radians), cos(radians), 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1);
    return rotz;

}

// @returns the rotation matrix around the vector and point by the specified angle
Matrix4x4 getRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {

    // @TODO: [CAMTRANS] Fill this in...
    REAL theta, phi;
    theta = atan2(v.z, v.x);
    phi = -atan2(v.y, sqrt(v.x*v.x + v.z*v.z));

    Matrix4x4 M1 = getRotYMat(theta);
    Matrix4x4 M2 = getRotZMat(phi);
    Matrix4x4 M3 = getRotXMat(a);

    Matrix4x4 invM1 = getInvRotYMat(theta);
    Matrix4x4 invM2 = getInvRotZMat(phi);

    Matrix4x4 Tran = getTransMat(p);
    Matrix4x4 invTran = getInvTransMat(p);

    Matrix4x4 M = Tran * invM1 * invM2 * M3 * M2* M1 * invTran;

    return M;

}


// @returns the inverse scale matrix described by the vector
Matrix4x4 getInvScaleMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    Matrix4x4 invscale(1/v.x, 0, 0, 0,
                    0, 1/v.y, 0, 0,
                    0, 0, 1/v.z, 0,
                    0, 0, 0, 1);
    return invscale;

}

// @returns the inverse translation matrix described by the vector
Matrix4x4 getInvTransMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    Matrix4x4 invtrans(1, 0, 0, -v.x,
                       0, 1, 0, -v.y,
                       0, 0, 1, -v.z,
                       0, 0, 0, 1);
    return invtrans;

}

// @returns the inverse rotation matrix about the x axis by the specified angle
Matrix4x4 getInvRotXMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    Matrix4x4 invrotx(1, 0, 0, 0,
                   0, cos(radians), sin(radians), 0,
                   0, -sin(radians), cos(radians), 0,
                   0, 0, 0, 1);
    return invrotx;

}

// @returns the inverse rotation matrix about the y axis by the specified angle
Matrix4x4 getInvRotYMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    Matrix4x4 invroty(cos(radians), 0, -sin(radians), 0,
                   0, 1, 0, 0,
                   sin(radians), 0, cos(radians), 0,
                   0, 0, 0, 1);
    return invroty;

}

// @returns the inverse rotatio-n matrix about the z axis by the specified angle
Matrix4x4 getInvRotZMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    Matrix4x4 invrotz(cos(radians), sin(radians), 0, 0,
                   -sin(radians), cos(radians), 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1);
    return invrotz;

}

// @returns the inverse rotation matrix around the vector and point by the specified angle
Matrix4x4 getInvRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {

    // @TODO: [CAMTRANS] Fill this in...
    REAL theta, phi;
    theta = atan2(v.z, v.x);
    phi = -atan2(v.y, sqrt(v.x*v.x + v.z*v.z));

    Matrix4x4 M1 = getRotYMat(theta);
    Matrix4x4 M2 = getRotZMat(phi);

    Matrix4x4 invM1 = getInvRotYMat(theta);
    Matrix4x4 invM2 = getInvRotZMat(phi);
    Matrix4x4 invM3 = getInvRotXMat(a);

    Matrix4x4 Tran = getTransMat(p);
    Matrix4x4 invTran = getInvTransMat(p);

    Matrix4x4 invM = Tran * invM1 * invM2 * invM3 * M2 * M1 * invTran;

    return invM;

}


//@}---------------------------------------------------------------------

