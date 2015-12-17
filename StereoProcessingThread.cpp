/**
 * Project 3DScanner
 */


#include "StereoProcessingThread.h"

/**
 * StereoProcessingThread implementation
 * 
 * Stereo processing thread for SGM. The Processing is started by calling the slot startProcessing(). The provided images are used to calculate the disparity maps.
 */

StereoProcessingThread::StereoProcessingThread() {
	m_thread.setParent(this);
	moveToThread(&m_thread);
	m_thread.start();
	m_isInitialized = false;
}
StereoProcessingThread::~StereoProcessingThread() {

}


/**
 * @param ...
 */
void StereoProcessingThread::initSGM(int width, int height, float maxDisp, int p1, int p2) {
	m_sgm.deinit();
	if(!m_sgm.init(width,height,maxDisp))
	{
		emit signalError("Can't init SGM");
		return;
	}
	m_sgm.updatePenalties(p1,p2);

	m_isInitialized = true;
}

/**
 * Starts proceesing and calculates disparity maps.
 * @param frameL
 * @param frameR
 */
void StereoProcessingThread::startProcessing(fcv::Image frameL, fcv::Image frameR, int id) {
	if(!m_isInitialized)
	{
		emit signalError("SGM not initialized!");
		return;
	}
	if (!m_sgm.processImagePair(&frameL, &frameR, true)) {
		emit signalError("Can't process SGM L2R.");
		return;
	}
	fcv::Image disp0(frameL.getWidth(), frameL.getHeight(), fcv::Image::PF_FLOAT_32, (unsigned char*) m_sgm.getDisparityData(), true);

	if (!m_sgm.processImagePair(&frameL, &frameR, false)) {
		emit signalError("Can't process SGM R2L.");
		return;
	}
	fcv::Image disp1(frameL.getWidth(), frameL.getHeight(), fcv::Image::PF_FLOAT_32, (unsigned char*) m_sgm.getDisparityData(), true);

	if (!m_sgm.l2rConsistencyCheck(&disp0, &disp1, 2)) {
		emit signalError("Can't process SGM consistency check");
		return;
	}

	emit signalDisparity(disp0,disp1, id);
}
