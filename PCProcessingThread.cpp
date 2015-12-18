/**
 * Project 3DScanner
 */


#include "PCProcessingThread.h"

/**
 * PCProcessingThread implementation
 */

PCProcessingThread::PCProcessingThread() {
	m_thread.setParent(this);
	moveToThread(&m_thread);
	m_thread.start();
	m_C = fcv::Vector2f();
	m_f = 0;
	m_b = 0;
}
PCProcessingThread::~PCProcessingThread() {

}
/**
 * @param ...
 */
void PCProcessingThread::initPCC(fcv::Vector2f C, float f, float b) {
	m_C = C;
	m_f = f;
	m_b = b;
	m_pcc.init(C,f,b);
}

/**
 * @param dispL
 * @param frameL
 */
void PCProcessingThread::startProcessing(fcv::Image disp, fcv::Image img, fcv::Matrix4x4f pose, int id) {
	fcv::Matrix4x4f H_opengl_opencv;

	H_opengl_opencv.setIdentity();
	H_opengl_opencv.at(1, 1) = -1;
	H_opengl_opencv.at(2, 2) = -1;

	pose = H_opengl_opencv * pose;

	m_pcc.setCameraPose(pose);

	fcv::PointCloudCreator::PointCloud points;
	if(!m_pcc.convertDisparity(&disp, &img, &points))
	{
		emit signalError("Can't create point cloud");
		return;
	}
	emit signalPointCloud(points, id);
}
