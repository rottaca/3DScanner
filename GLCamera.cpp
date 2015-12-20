/**
 * Project 3DScanner
 */

#include "GLCamera.h"
#include <QtCore/qmath.h>

#include <FreeCV/FreeCV.h>

/**
 * GLCamera implementation
 */

void GLCamera::setupProjection(float verticalAngle,float aspectRatio, float near, float far)
{
	m_projMatrx.setToIdentity();
	m_projMatrx.perspective(verticalAngle, aspectRatio, near, far);
}

void GLCamera::setupView(QVector3D eye, QVector3D center, QVector3D up)
{
	m_viewMatrix.setToIdentity();
	m_viewMatrix.lookAt(eye,center,up);
	m_eye = eye;
	m_center = center;
	m_up = up;
}

void GLCamera::turnLR(float amount)
{
	QVector3D lookDir = m_center - m_eye;
	lookDir.normalize();
	QVector3D rightDir = QVector3D::crossProduct(lookDir,m_up);
	rightDir.normalize();
	QVector3D realUp =  QVector3D::crossProduct(rightDir,lookDir);
	realUp.normalize();

	QMatrix4x4 R;
	R.rotate(amount,realUp);

	lookDir = R*lookDir;
	m_center = m_eye + lookDir;
	recalculateCamera();
}

void GLCamera::turnUD(float amount)
{
	QVector3D lookDir = m_center - m_eye;
	lookDir.normalize();
	QVector3D rightDir = QVector3D::crossProduct(lookDir,m_up);
	rightDir.normalize();

	QMatrix4x4 R;
	R.rotate(amount,rightDir);

	lookDir = (R*lookDir.toVector4D()).toVector3D();

	// Maximum look angle +/- 89,99 Deg
	float angle = fabs(qRadiansToDegrees(acos(QVector3D::dotProduct(m_up,lookDir.normalized()))));

	LOG_FORMAT_INFO("Angle: %f", angle);

	if(angle > 1 && angle < 179)
	{
		m_center = m_eye + lookDir;
		recalculateCamera();
	}
}

void GLCamera::moveFWBW(float amount)
{
	QVector3D lookDir = m_center - m_eye;
	lookDir.normalize();
	m_center += lookDir*amount;
	m_eye += lookDir*amount;
	recalculateCamera();
}
void GLCamera::moveUD(float amount)
{
	QVector3D lookDir = m_center - m_eye;
	lookDir.normalize();
	QVector3D rightDir = QVector3D::crossProduct(lookDir,m_up);
	rightDir.normalize();
	QVector3D realUp =  QVector3D::crossProduct(rightDir,lookDir);
	realUp.normalize();

	m_center += realUp*amount;
	m_eye += realUp*amount;
	recalculateCamera();
}
void GLCamera::moveLR(float amount)
{
	QVector3D lookDir = m_center - m_eye;
	lookDir.normalize();
	QVector3D rightDir = QVector3D::crossProduct(lookDir,m_up);
	rightDir.normalize();

	m_center -= rightDir*amount;
	m_eye -= rightDir*amount;
	recalculateCamera();
}
void GLCamera::recalculateCamera()
{
	m_viewMatrix.setToIdentity();
	m_viewMatrix.lookAt(m_eye, m_center, m_up);
}
