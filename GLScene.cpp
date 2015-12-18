/**
 * Project 3DScanner
 */


#include "GLScene.h"

/**
 * GLScene implementation
 */


GLScene::GLScene(QWidget* parent): QOpenGLWidget(parent)
{

}
GLScene::~GLScene()
{

}
/**
 * @param obj
 */
void GLScene::addDrawable(Drawable obj) {

}

void GLScene::removeDrawables() {

}

void GLScene::renderScene() {

}

void GLScene::initializeGL()
{
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
}
void GLScene::paintGL()
{
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
}
void GLScene::resizeGL(int width, int height)
{

}
