Name : Siqi Ma
Brown ID: 100641905
CS login name: dollymsq
E_mail: siqi_ma@brown.edu

Taking CS1234 only
======================= Ray =================
Extra
1. Multithread. I use the QObject::idealThreadCount() function to find out the optimal thread number. 
2. Kd-tree. I implemented the kd-tree as described in the pdf. I didn't optimize the running time though. When the scene primitive number is small, it would actually take a longer time that before since it has to intersect many bounding boxes besides primitives. But when ray-tracing the recursivesphere file, you can see its great benefit.
3. Adaptive Supersampling. I combined the stochastic sampling with the adaptive supersampling. Stochastic sampling is manily used to find samples in a patch. But it's still relatively slow.

4. Additional anti-aliasing. I did a edge detection and blur the edges.
4. Texture linear filtering. When raytracing with refract2.xml, you can see a clear improvement in texture from linear filtering.
5. Refraction. I am not sure whether it's comletely correct. The effect seems good with refract1.xml (its primitives have a relatively smaller ior), but a little weird with refract2.xml( I guess maybe it's because the texture, relatively strong diffuse color and large ior).


======================= Intersect ==============
1. Didn't do much extra this time, would do extra with raytracing assignment.
2. shadow and reflect with black dots now.

======================= Sceneview ==============
1. Count the items in the scene before rendering to decide parameters for shapes.
2. OpenGL state error when rendering xml file
3. Texture not working now.

======================= Filter ==============
1. Implemented extra rotate
2. Median
3. Basic Bilateral smoothing (quite slow)
4. Auto-level (not performing well when handle very colorful picture, but pretty good at single tone ones.)

I have attached the mirkwood.jpg file for you to see the good auto-level effect. It also work well with the autumn.jpg in data/image file.

taf-rbg-noise.jpg is for Median filter.


======================== Shapes ===========
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

