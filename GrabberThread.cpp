/**
 * Project 3DScanner
 */

#include "GrabberThread.h"

/**
 * GrabberThread implementation
 * 
 * Controls the VideoCapture device and receives images form the devices. Stereo Images will be provided by a qt signal.
 */

GrabberThread::GrabberThread() {
	m_thread.setParent(this);
	moveToThread(&m_thread);
	m_timer.moveToThread(&m_thread);
	m_thread.start();
	m_timer.setInterval(10);
	m_isCaptureing = false;

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(grabFrame()));
}
GrabberThread::~GrabberThread() {
	m_grabber1.closeDev();
	m_grabber2.closeDev();
	m_thread.terminate();
}
/**
 * Stores the device names for the cams in member variables. But does no Initialization.
 * 
 * @param camL
 * @param camR
 */
bool GrabberThread::initGrabber(std::string camL, std::string camR) {
	if (!m_grabber1.openAndInitDev(camL) || !m_grabber2.openAndInitDev(camR))
		return false;

	return true;
}

/**
 * Stops the grabbing and closes the device files.
 */
void GrabberThread::stopCapture() {
	m_isCaptureing = false;
	m_timer.stop();
	m_grabber1.stopCapture();
	m_grabber2.stopCapture();
}

/**
 * Opens and initializes the driver and starts grabbing.
 */
void GrabberThread::startCapture() {
	if(!m_grabber1.startCapture() || !m_grabber2.startCapture())
		return;

//	m_exposureCtrl1.setupExposureControl(&m_grabber1,fcv::ExposureController::AUTOMATIC);
//	m_exposureCtrl2.setupExposureControl(&m_grabber2,fcv::ExposureController::AUTOMATIC);
	m_timer.start();
	m_isCaptureing = true;
}

/**
 * Switches between exposure modes: manual (fixed exp), manual (fixed goal value, automatic ctrl), automatic mode (use camera own exposure).
 * @param mode
 */
void GrabberThread::changeExposureMode(fcv::ExposureController::ExposureCtrlMode mode) {
	m_exposureCtrl1.setupExposureControl(&m_grabber1,mode,500);
	m_exposureCtrl2.setupExposureControl(&m_grabber2,mode,500);
}

/**
 * Adjusts the exposure value according to the exposure mode.
 * @param val
 */
void GrabberThread::changeExposureValue(int val) {
	m_exposureCtrl1.updateGoalVal(val);
	m_exposureCtrl2.updateGoalVal(val);
}

void GrabberThread::grabFrame() {
	fcv::Image imgLeftRaw, imgRightRaw;
	if (m_grabber1.isCaptureing() && m_grabber1.grabFrame(&imgLeftRaw) && m_grabber2.grabFrame(&imgRightRaw)) {
		if (imgLeftRaw.getFormat() == fcv::Image::PF_YUYV) {
			fcv::Image imgLeftRGB, imgRightRGB;
			fcv::convertPxFormat(&imgLeftRaw, &imgLeftRGB, fcv::YUYV_TO_RGB_888);
			fcv::convertPxFormat(&imgRightRaw, &imgRightRGB, fcv::YUYV_TO_RGB_888);
			// TODO Exposure ctrl

			emit signalNewFrame(imgLeftRGB, imgRightRGB);
		} else {
			emit signalNewFrame(imgLeftRaw, imgRightRaw);
		}
	} else {
		LOG_INFO("Can't grab frame");
	}
}
