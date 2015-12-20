/**
 * Project 3DScanner
 */


#include "GLScene.h"
#include <FreeCV/FreeCV.h>

#include "DrawableDrawablePointCloud.h"
/**
 * GLScene implementation
 */


GLScene::GLScene(QWidget* parent): QOpenGLWidget(parent)
{
	m_attrCol = -1;
	m_attrPos = -1;

	//set MouseTracking true to capture mouse event even its key is not pressed
	setMouseTracking(true);
	isPressedML = false;
	isPressedMR = false;
	isPressedMM = false;
}
GLScene::~GLScene()
{

}
/**
 * @param obj
 */
void GLScene::addDrawable(Drawable* obj) {
	LOG_INFO("Adding drawable");

	m_drawableObj.push_back(obj);
	repaint();
}

void GLScene::removeDrawables() {
	LOG_INFO("Removing drawable");
	for (int i = 0; i < m_drawableObj.size(); i++) {
		delete m_drawableObj.at(i);
	}
	m_drawableObj.clear();
}

void GLScene::initializeGL()
{
	LOG_INFO("initializeGL");
	m_cam.setupView(QVector3D(0,0,1),QVector3D(0,0,0),QVector3D(0,1,0));

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!m_shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, "vertex.vert")) {
		emit signalError("Can't load vertex shader!");
		return;
	}
	if (!m_shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, "fragment.frag")) {
		emit signalError("Can't load fragment shader!");
		return;
	}
	if (!m_shaderProg.link()) {
		emit signalError("Can't load fragment shader!");
		return;
	}
	if (!m_shaderProg.bind()) {
		emit signalError("Can't bind shader program!");
		return;
	}

	// Create Ground Plane
	// Position and color
	m_groundVertices.clear();
	float groundplaneHalfSize = 0.5;
	float transparency = 0.1;
	m_groundVertices << QVector4D(-groundplaneHalfSize, -groundplaneHalfSize,  0.0f, 1.0f) << QVector4D(1.0f, 1.0f, 1.0f, transparency);
	m_groundVertices << QVector4D(-groundplaneHalfSize, groundplaneHalfSize,  0.0f, 1.0f) << QVector4D(1.0f, 1.0f, 1.0f, transparency);
	m_groundVertices << QVector4D( groundplaneHalfSize, -groundplaneHalfSize,  0.0f, 1.0f) << QVector4D(1.0f, 1.0f, 1.0f, transparency);
	m_groundVertices << QVector4D( groundplaneHalfSize, -groundplaneHalfSize,  0.0f, 1.0f) << QVector4D(1.0f, 1.0f, 1.0f, transparency);
	m_groundVertices << QVector4D(-groundplaneHalfSize, groundplaneHalfSize,  0.0f, 1.0f) << QVector4D(1.0f, 1.0f, 1.0f, transparency);
	m_groundVertices << QVector4D( groundplaneHalfSize, groundplaneHalfSize,  0.0f, 1.0f) << QVector4D(1.0f, 1.0f, 1.0f, transparency);

	m_vaoGround.create();
	m_vaoGround.bind();

	m_vboGround.create();
	m_vboGround.bind();
	m_vboGround.setUsagePattern(QOpenGLBuffer::StaticDraw );
	m_vboGround.allocate(m_groundVertices.constData(),  m_groundVertices.size()*sizeof(QVector4D));

	m_attrPos = m_shaderProg.attributeLocation("vPos");
	m_attrCol = m_shaderProg.attributeLocation("vCol");
	m_uniMVP = m_shaderProg.uniformLocation("mMVP");
	m_uniModelMatrix = m_shaderProg.uniformLocation("mModelMatrix");

	if (m_attrPos == -1 || m_attrCol == -1 || m_uniMVP == -1) {
		emit signalError("Can't find all attributes in shader!");
		return;
	}
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(m_attrPos);
	f->glEnableVertexAttribArray(m_attrCol);
	f->glVertexAttribPointer(m_attrPos, 4, GL_FLOAT, GL_FALSE, 2*sizeof(QVector4D), 0);
	f->glVertexAttribPointer(m_attrCol, 4, GL_FLOAT, GL_FALSE, 2*sizeof(QVector4D), reinterpret_cast<void *>(sizeof(QVector4D)));

	m_vboGround.release();

	m_vaoGround.release();

//	fcv::PointCloudCreator::PointCloud pc;
//	fcv::PointCloudCreator::PixelData pd,pd2,pd3;
//	pd.color[1] = 255;
//	pd.pos[0] = 1;
//	pd2.color[0] = 255;
//	pd2.pos[1] = 1;
//	pd3.color[2] = 255;
//	pd3.pos[2] = 1;
//	pc.push_back(pd);
//	pc.push_back(pd2);
//	pc.push_back(pd3);
//	DrawablePointCloud* draw = new DrawablePointCloud;
//	draw->setPointCloudData(pc,2);
//	addDrawable(draw);

}
void GLScene::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QMatrix4x4 mMVP = m_cam.getProjectionMatrix()*m_cam.getViewMatrix();
	QMatrix4x4 mModelmatrix;
	mModelmatrix.setToIdentity();
	m_shaderProg.setUniformValue(m_uniMVP, mMVP);
	m_shaderProg.setUniformValue(m_uniModelMatrix, mModelmatrix);

	// Draw pc
	for(int i = 0; i < m_drawableObj.size(); i++){
		// TODO Add model matrix
		if (!m_drawableObj.at(i)->createBuffers(m_attrPos, m_attrCol)) {
			emit signalError("Can't create buffers of drawable!");
			return;
		}
		m_drawableObj.at(i)->drawData();
	}

	m_vaoGround.bind();
	glDrawArrays(GL_TRIANGLES,0, 6);
	m_vaoGround.release();

}
void GLScene::resizeGL(int width, int height)
{
//	LOG_INFO("resizeGL");
	// Set the viewport to window dimensions
	glViewport(0, 0, width, height);
	m_cam.setupProjection(60.0f, GLfloat(width) / height, 0.1f, 50.0f);
//	LOG_INFO("done");
}
void GLScene::mouseMoveEvent(QMouseEvent *ev)
{
	if(!isPressedML && !isPressedMM && !isPressedMR)
		return;

	QPointF newPos = ev->pos();
	QPointF delta = newPos-m_oldMousePos;
	LOG_FORMAT_INFO("%f %f", delta.x(),delta.y());

	if(isPressedML)
	{
		LOG_INFO("Left Button pressed");

		m_cam.turnUD(delta.y()/10.0);
		m_cam.turnLR(delta.x()/10.0);

	}
	else if(isPressedMR)
	{
		LOG_INFO("Right Button pressed");
		m_cam.moveFWBW(delta.y()/100.0);
	}
	else if(isPressedMM)
	{
		LOG_INFO("Center Button pressed");

		m_cam.moveUD(delta.y()/100.0);
		m_cam.moveLR(delta.x()/100.0);
	}
	m_oldMousePos = newPos;

	repaint();
}

void GLScene::mousePressEvent(QMouseEvent *event)
{
	m_oldMousePos = event->pos();

	if (event->buttons() == Qt::LeftButton) {
		isPressedML = true;
	}
	if (event->buttons() == Qt::RightButton) {
		isPressedMR = true;
	}
	if (event->buttons() == Qt::MiddleButton) {
		isPressedMM = true;
	}
}
void GLScene::mouseReleaseEvent(QMouseEvent *event) {
	if (event->buttons() != Qt::LeftButton) {
		isPressedML = false;
	}
	if (event->buttons() != Qt::RightButton) {
		isPressedMR = false;
	}
	if (event->buttons() != Qt::MiddleButton) {
		isPressedMM = false;
	}
}

