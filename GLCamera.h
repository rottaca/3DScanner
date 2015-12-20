/**
 * Project 3DScanner
 */


#ifndef _GLCAMERA_H
#define _GLCAMERA_H

#include <QtGui/qmatrix4x4.h>

class GLCamera {
public:
	GLCamera(){
		m_projMatrx.setToIdentity();
		m_viewMatrix.setToIdentity();
	}
	~GLCamera(){}

	void setupProjection(float verticalAngle,float aspectRatio, float near, float far);
	void setupView(QVector3D eye, QVector3D center, QVector3D up);

	void turnLR(float amount);
	void turnUD(float amount);

	void moveFWBW(float amount);
	void moveUD(float amount);
	void moveLR(float amount);

	QMatrix4x4 getViewMatrix(){return m_viewMatrix;}
	QMatrix4x4 getProjectionMatrix(){return m_projMatrx;}

	void recalculateCamera();
private:
	QMatrix4x4 m_projMatrx, m_viewMatrix;
	QVector3D m_eye;
	QVector3D m_center;
	QVector3D m_up;
};

#endif //_GLCAMERA_H
