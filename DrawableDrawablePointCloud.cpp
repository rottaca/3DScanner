/**
 * Project 3DScanner
 */


#include "DrawableDrawablePointCloud.h"

/**
 * PointCloud implementation
 */

DrawablePointCloud::DrawablePointCloud() {
	m_pointSize = 1;
}
DrawablePointCloud::~DrawablePointCloud() {

}

bool DrawablePointCloud::createBuffers(int attrPos, int attrCol) {


////	m_pointCloud.clear();
//	fcv::PointCloudCreator::PixelData pd, pd2, pd3;
//	pd.color[1] = 255;
//	pd.pos[0] = 1;
//	pd2.color[0] = 255;
//	pd2.pos[1] = 1;
//	pd3.color[2] = 255;
//	pd3.pos[2] = 1;
//	m_pointCloud.push_back(pd);
//	m_pointCloud.push_back(pd2);
//	m_pointCloud.push_back(pd3);

	if(attrPos == -1 || attrCol == -1)
		return false;
	if(m_vao.isCreated())
		return true;

	m_vert.clear();
	for(int i = 0; i < m_pointCloud.size(); i++)
	{
		fcv::PointCloudCreator::PixelData& pd = m_pointCloud.at(i);
		m_vert << QVector4D(pd.pos[0],pd.pos[1],pd.pos[2], 1.0f);
//		m_vert << QVector4D(0.5,0,0.5, 1.0f);
		m_vert << QVector4D(pd.color[0]/255, pd.color[1]/255, pd.color[2]/255, 1.0f);
//		m_vert << QVector4D(0, 1, 0 ,1.0f);
	}

	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vbo.allocate(m_vert.constData(), m_vert.size() * sizeof(QVector4D));

	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(attrPos);
	f->glEnableVertexAttribArray(attrCol);
	f->glVertexAttribPointer(attrPos, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(QVector4D), 0);
	f->glVertexAttribPointer(attrCol, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(QVector4D), reinterpret_cast<void *>(sizeof(QVector4D)));

	m_vbo.release();

	m_vao.release();
	return true;
}

void DrawablePointCloud::drawData() {

	m_vao.bind();
	glPointSize(m_pointSize);
	glDrawArrays(GL_POINTS,0, m_pointCloud.size());
//	for(int i = 0; i < m_vert.size(); i++)
//	{
//		QVector4D pd = m_vert.at(i);
//		LOG_FORMAT_INFO("%f %f %f",pd[0],pd[1],pd[2]);
//	}
	m_vao.release();
}

/**
 * @param pc
 */
void DrawablePointCloud::setPointCloudData(fcv::PointCloudCreator::PointCloud pc, float pointSize) {
	m_pointCloud = pc;
	m_pointSize = pointSize;
}
