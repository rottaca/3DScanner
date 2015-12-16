cmake_minimum_required (VERSION 2.6)
PROJECT(3DScanner)

find_package( OpenCV REQUIRED )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/opt/qt/5.5/gcc_64")
# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)


if(Qt5Widgets_FOUND)
    add_definitions(-DQT_FOUND)
endif()

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

SET(SOURCE_FILES 
    Ctrl.cpp
    GLCamera.cpp
    GLScene.cpp
    GrabberThread.cpp
    Gui.cpp
    HardwareController.cpp
    HardwareControllerThread.cpp
    PCProcessingThread.cpp
    StereoFrameData.cpp 
    StereoProcessingThread.cpp
    MyQListWidgetItem.cpp
)

SET(HEADER_FILES
)

QT5_WRAP_UI(UIS_HDRS
    mainform.ui
)

ADD_EXECUTABLE(3DScanner ${SOURCE_FILES} ${HEADER_FILES} ${UIS_HDRS})


set_target_properties(3DScanner PROPERTIES AUTOMOC TRUE)
qt5_use_modules(3DScanner Widgets OpenGL)

TARGET_LINK_LIBRARIES(3DScanner FreeCV ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
