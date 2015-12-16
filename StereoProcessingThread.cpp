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
}
StereoProcessingThread::~StereoProcessingThread() {

}


/**
 * @param ...
 */
void StereoProcessingThread::initSGM() {

}

/**
 * Starts proceesing and calculates disparity maps.
 * @param frameL
 * @param frameR
 */
void StereoProcessingThread::startProcessing(fcv::Image frameL, fcv::Image frameR) {

}
