#include "Shape.h"

Shape::Shape(int p1, int p2, double p3)
{
    m_p1 = p1;
    m_p2 = p2;
    m_p3 = p3;
    vertexData = NULL;

    // Initialize the vertex array object.
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);
    glGenBuffers(1, &vertexBuffer);

}

Shape::Shape(Vector4 pp, Vector4 dd)
{
    p = pp;
    d = dd;
}

Shape::~Shape()
{
//    m_pList.clear();
//    m_nList.clear();
//    m_tList.clear();

//    glDeleteBuffers(m_vsize, &vertexBuffer);

}

void Shape::drawTriangle(int offset1, int offset2, int offset3, int j)
{
    it = m_pList.begin() + offset1;
    Vector3 v1 = *it;
    it = m_nList.begin() + offset1;
    Vector3 n1 = *it;

    it = m_pList.begin() + offset2;
    Vector3 v2 = *it;
    it = m_nList.begin() + offset2;
    Vector3 n2 = *it;

    it = m_pList.begin() + offset3;
    Vector3 v3 = *it;
    it = m_nList.begin() + offset3;
    Vector3 n3 = *it;

    itc = m_cList.begin()+offset1;
    Vector2 c1 = *itc;

    itc = m_cList.begin()+offset2;
    Vector2 c2 = *itc;

    itc = m_cList.begin()+offset3;
    Vector2 c3 = *itc;

    vertexData[24*j + 0] = float(v1.x); vertexData[24*j + 1] = float(v1.y); vertexData[24*j + 2] = float(v1.z);
    vertexData[24*j + 3] = float(n1.x); vertexData[24*j + 4] = float(n1.y); vertexData[24*j + 5] = float(n1.z);
    vertexData[24*j + 6] = float(c1.x); vertexData[24*j + 7] = float(c1.y);

    vertexData[24*j + 8] = float(v2.x); vertexData[24*j + 9] = float(v2.y); vertexData[24*j + 10] = float(v2.z);
    vertexData[24*j + 11] = float(n2.x); vertexData[24*j + 12] = float(n2.y); vertexData[24*j + 13] = float(n2.z);
    vertexData[24*j + 14] = float(c2.x); vertexData[24*j + 15] = float(c2.y);

    vertexData[24*j + 16] = float(v3.x); vertexData[24*j + 17] = float(v3.y); vertexData[24*j + 18] = float(v3.z);
    vertexData[24*j + 19] = float(n3.x); vertexData[24*j + 20] = float(n3.y); vertexData[24*j + 21] = float(n3.z);
    vertexData[24*j + 22] = float(c3.x); vertexData[24*j + 23] = float(c3.y);

}

void Shape::drawShape()
{
    int j;

    for(iit = m_tList.begin(), j = 0; iit != m_tList.end(); iit += 3, j ++)
    {
        drawTriangle(*iit, *(iit+1), *(iit+2), j);
    }
}

REAL Shape::calculateCapItsP(Vector3 n, Vector3 p0)
{
    return ((n.x*p0.x + n.y*p0.y+n.z*p0.z) - (n.x*p.x + n.y*p.y + n.z*p.z))/(n.x*d.x + n.y*d.y + n.z*d.z);
}

void Shape::getEquationRoot(REAL a, REAL b, REAL c, REAL &t1, REAL &t2)
{
    REAL delta;
    delta = b*b - 4*a*c;
    if(delta >= 0)
    {
        t1 = (-b + sqrt(delta))/2.0/a;
        t2 = (-b - sqrt(delta))/2.0/a;
    }
}

void Shape::checkBodyBoundary(REAL tmpt, REAL &tmin, Vector3 n, Vector3 &tnormal)
{
    if(tmpt >= 0 )
    {
        REAL y = p.y + tmpt*d.y;
        if(y > 0.5 || y < -0.5)
            tmpt = MAX_LIMIT;
        else
        {
            if(tmpt < tmin)
            {
                tmin = tmpt;
                tnormal = n;
            }
        }
    }
}

void Shape::checkCircleCapBoundary(REAL tmpt, REAL &tmin, Vector3 n, Vector3 &tnormal)
{
    REAL x = p.x + tmpt*d.x;
    REAL z = p.z + tmpt*d.z;

    if(x*x + z*z > 0.25 || tmpt < 0)
        tmpt = MAX_LIMIT;
    else
    {
        if(tmin > tmpt)
        {
            tmin = tmpt;
            tnormal = n;
        }
    }
}

int Shape::getVerticesNumber()
{
    return m_tList.size();
}

void Shape::packVerticesintoBuffer(GLuint &m_shader)
{
        glBindVertexArray(m_vaoID);

        m_vsize = getVerticesNumber();

        vertexData = new GLfloat[m_vsize*8];
        //fill in the vertexData
        drawShape();

        // Pass vertex data to OpenGL.
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        glBufferData(GL_ARRAY_BUFFER, m_vsize * 8 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

        glEnableVertexAttribArray(glGetAttribLocation(m_shader, "position"));
        glEnableVertexAttribArray(glGetAttribLocation(m_shader, "normal"));
        glEnableVertexAttribArray(glGetAttribLocation(m_shader, "texCoord"));

        glVertexAttribPointer(
                    glGetAttribLocation(m_shader, "position"),
                    3,                   // Num coordinates per position
                    GL_FLOAT,            // Type
                    GL_FALSE,            // Normalized
                    sizeof(GLfloat) * 8, // Stride
                    (void*) 0            // Array buffer offset
                    );
        glVertexAttribPointer(
                    glGetAttribLocation(m_shader, "normal"),
                    3,           // Num coordinates per normal
                    GL_FLOAT,    // Type
                    GL_TRUE,     // Normalized
                    sizeof(GLfloat) * 8,           // Stride
                    (void*) (sizeof(GLfloat) * 3)    // Array buffer offset
                    );
        glVertexAttribPointer(
                    glGetAttribLocation(m_shader, "texCoord"),
                    2,           // Num coordinates per texcoord
                    GL_FLOAT,    // Type
                    GL_TRUE,     // Normalized
                    sizeof(GLfloat) * 8,           // Stride
                    (void*) (sizeof(GLfloat) * 6)    // Array buffer offset
                    );


        // Unbind buffers.
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}



//Vector2 Shape::calculateTexCoor(Vector2 tex)
//{
//    Vector2 t;
//    t.x = ((int)(tex.x*m_material.textureMap->repeatU*texImgWidth))%texImgWidth;
//    t.y = ((int)(tex.y*m_material.textureMap->repeatV*texImgHeight))%texImgHeight;
//    return t;
//}
