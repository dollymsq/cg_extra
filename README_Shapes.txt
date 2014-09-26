Name : Siqi Ma
Brown ID: 100641905
CS login name: dollymsq
E_mail: siqi_ma@brown.edu

Taking CS1234 only

1. I created a base class shape, and I developed class cube, cylinder, sphere, cone, torus and fractal from it.
I set several virtual functions in this class.
I have three deques in this class, 
(1)m_pList for points, 
(2)m_tList for triangles,
(3)m_nList for normals. 
Basically a vertex has the same index in the deque m_pLlist and m_nList. It's convenient in this way to render normals and draw triangles. The m_tList store the triangle information, it stored the index of points which made up triangles.
After generating points of shapes in this organized way, I store the triangle information in m_tList.

For Extra:
1. Torus
2. Fractal

