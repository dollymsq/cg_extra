# -------------------------------------------------
# Project created by QtCreator 2010-08-22T14:12:19
# -------------------------------------------------
QT += opengl xml
TARGET = CS123
TEMPLATE = app
unix:!macx {
    LIBS += -lGLU
    QMAKE_CXXFLAGS += -std=c++11
}
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
}
SOURCES += brush/SmudgeBrush.cpp \
    brush/QuadraticBrush.cpp \
    brush/LinearBrush.cpp \
    brush/ConstantBrush.cpp \
    brush/Brush.cpp \
    camera/OrbitingCamera.cpp \
    camera/CamtransCamera.cpp \
    scenegraph/SelectionRecorder.cpp \
    lib/CS123XmlSceneParser.cpp \
    lib/ResourceLoader.cpp \
    scenegraph/ShapesScene.cpp \
    scenegraph/SceneviewScene.cpp \
    scenegraph/Scene.cpp \
    scenegraph/RayScene.cpp \
    scenegraph/OpenGLScene.cpp \
    ui/SupportCanvas3D.cpp \
    ui/SupportCanvas2D.cpp \
    ui/Settings.cpp \
    ui/mainwindow.cpp \
    ui/Databinding.cpp \
    ui/Canvas3D.cpp \
    ui/Canvas2D.cpp \
    main.cpp \
    brush/TubeBrush.cpp \
    shape/Sphere.cpp \
    shape/Shape.cpp \
    shape/Cylinder.cpp \
    shape/Cube.cpp \
    shape/Cone.cpp \
    filter/Filter.cpp \
    filter/FScale.cpp \
    filter/FMedian.cpp \
    filter/FInvert.cpp \
    filter/FGrayScale.cpp \
    filter/FEdgeDetect.cpp \
    filter/FBlur.cpp \
    filter/FAutoLevel.cpp \
    shape/Torus.cpp \
    shape/Fractal.cpp \
    filter/FRotate.cpp \
    filter/FBilateral.cpp


HEADERS += brush/SmudgeBrush.h \
    brush/QuadraticBrush.h \
    brush/LinearBrush.h \
    brush/ConstantBrush.h \
    brush/Brush.h \
    camera/OrbitingCamera.h \
    camera/CamtransCamera.h \
    camera/Camera.h \
    scenegraph/SelectionRecorder.h \
    lib/CS123XmlSceneParser.h \
    lib/CS123SceneData.h \
    lib/CS123ISceneParser.h \
    lib/CS123Common.h \
    lib/ResourceLoader.h \
    scenegraph/ShapesScene.h \
    scenegraph/SceneviewScene.h \
    scenegraph/Scene.h \
    scenegraph/RayScene.h \
    scenegraph/OpenGLScene.h \
    ui/SupportCanvas3D.h \
    ui/SupportCanvas2D.h \
    ui/Settings.h \
    ui/mainwindow.h \
    ui/Databinding.h \
    ui/Canvas3D.h \
    ui/Canvas2D.h \
    ui_mainwindow.h \
    brush/TubeBrush.h \
    shape/Sphere.h \
    shape/Shape.h \
    shape/Cylinder.h \
    shape/Cube.h \
    shape/Cone.h \
    filter/Filter.h \
    filter/FScale.h \
    filter/FMedian.h \
    filter/FInvert.h \
    filter/FGrayScale.h \
    filter/FEdgeDetect.h \
    filter/FBlur.h \
    filter/FAutoLevel.h \
    shape/Torus.h \
    shape/Fractal.h \
    filter/FRotate.h \
    filter/FBilateral.h


FORMS += ui/mainwindow.ui
INCLUDEPATH += glm brush camera lib scenegraph ui
DEPENDPATH += glm brush camera lib scenegraph ui
DEFINES += TIXML_USE_STL
OTHER_FILES += shaders/shader.frag shaders/shader.vert

# Don't add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wformat\
                          -Wmain -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type \
                          -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch \
                          -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-variable \
                          -Wvolatile-register-var -Wno-extra
# QMAKE_CXX_FLAGS_WARN_ON += -Wunknown-pragmas -Wunused-function -Wmain

macx {
    QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds -Wc++0x-compat
}

#TODO (Windows): If you are setting up local development on Windows (NOT Mac), comment out the following lines
win32:CONFIG(release, debug|release): LIBS += -L/course/cs123/lib/glew/glew-1.10.0/lib/release/ -lGLEW
else:win32:CONFIG(debug, debug|release): LIBS += -L/course/cs123/lib/glew/glew-1.10.0/lib/debug/ -lGLEW
else:unix: LIBS += -L/Users/masiqi/glew-1.10.0/lib/ -lGLEW

#TODO (Windows): If you are setting up local development on Windows (NOT Mac), fill in the correct name of your glew library (the .a file) and uncomment the following lines:
#DEFINES += GLEW_STATIC
#LIBS += -l<name of your glew library> -lopengl32


#TODO (Windows or Mac): If you are setting up local development on Windows OR Mac, fill in the correct path to your glew and uncomment the following lines:
INCLUDEPATH+=/Users/masiqi/glew-1.10.0/include
DEPENDPATH+=/Users/masiqi/glew-1.10.0/include

RESOURCES += \
    resources.qrc
