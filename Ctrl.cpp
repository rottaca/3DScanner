/**
 * Project 3DScanner
 */

#include "Ctrl.h"
#include "Gui.h"

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmessagebox.h>
/**
 * Ctrl implementation
 */

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	new Ctrl();
	return app.exec();
}

Ctrl::Ctrl() {
	m_gui = new Gui(this);
	m_maxSeqNr = 1;
	m_opMode = OM_IDLE;

	connectSignals();
}
Ctrl::~Ctrl() {
	delete m_gui;
}

void Ctrl::connectSignals() {
	qRegisterMetaType<fcv::Image>("fcv::Image");
	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<fcv::PointCloudCreator::PointCloud>("fcv::PointCloudCreator::PointCloud");
	qRegisterMetaType<fcv::ExposureController::ExposureCtrlMode>("fcv::ExposureController::ExposureCtrlMode");

	// Slots
	connect(&m_grabber, SIGNAL(signalNewFrame(fcv::Image, fcv::Image)), this, SLOT(receiveNewFrame(fcv::Image, fcv::Image)));
	connect(&m_grabber, SIGNAL(signalError(std::string)), this, SLOT(displayError(std::string)));

	connect(&m_hwCtrl, SIGNAL(signalRotated(float)), this, SLOT(receivePlatformRotated(float)));
	connect(&m_hwCtrl, SIGNAL(signalError(std::string)), this, SLOT(displayError(std::string)));

	connect(&m_pcProc, SIGNAL(signalPointCloud(fcv::PointCloudCreator::PointCloud)), this, SLOT(receivePointCloud(fcv::PointCloudCreator::PointCloud)));
	connect(&m_pcProc, SIGNAL(signalError(std::string)), this, SLOT(displayError(std::string)));

	connect(&m_stereoProc, SIGNAL(signalDisparity(fcv::Image, fcv::Image)), this, SLOT(receiveDispairty(fcv::Image, fcv::Image)));
	connect(&m_stereoProc, SIGNAL(signalError(std::string)), this, SLOT(displayError(std::string)));

	// Signals
	connect(this, SIGNAL(signalStartCapture()), &m_grabber, SLOT(startCapture()));
	connect(this, SIGNAL(signalStopCapture()), &m_grabber, SLOT(stopCapture()));

	connect(this, SIGNAL(signalChangeExposureMode(fcv::ExposureController::ExposureCtrlMode)), &m_grabber,
			SLOT(changeExposureMode(fcv::ExposureController::ExposureCtrlMode)));
	connect(this, SIGNAL(signalChangeExposureValue(int)), &m_grabber, SLOT(changeExposureValue(int)));

}

/**
 * Displays an error message
 * @param errStr
 */
void Ctrl::displayError(std::string errStr) {
	QMessageBox::critical(m_gui, "Error", errStr.c_str());
}

/**
 * Loads all camera calibration files.
 */
void Ctrl::loadCalibrationFiles() {

}

/**
 * slot for receiving the new stereo image pair
 * @param frameL
 * @param frameR
 */
void Ctrl::receiveNewFrame(fcv::Image frameL, fcv::Image frameR) {

	preprocessFrames(frameL,frameR);
	m_gui->displayStereoImage(frameL, frameR, Ctrl::TAB_CAPTURE);
	if(m_opMode == OM_CAPTURE_FRAME_SEQUENCE){
		StereoFrameData data;
		data.setFrame(frameL, frameR);

		m_frameDataSequenze.push_back(data);

		if(m_frameDataSequenze.size() >= m_maxSeqNr)
			m_opMode = OM_FREE_RUN_CAPTURE;

		LOG_FORMAT_INFO("Added frame %d of %d",m_frameDataSequenze.size(),m_maxSeqNr);
	}
}

/**
 * slot for receiving next disparity image pair
 * @param dispL
 * @param dispR
 */
void Ctrl::receiveDispairty(fcv::Image dispL, fcv::Image dispR) {

}

/**
 * slot for receiving the new point cloud
 * @param pcL
 */
void Ctrl::receivePointCloud(fcv::PointCloudCreator::PointCloud pcL) {

}

/**
 * slot that is called when hardware platform rotation is executed completely.
 * @param angle
 */
void Ctrl::receivePlatformRotated(float angle) {

}

/**
 * Called when stop capture is clicked
 */
void Ctrl::onClickStopCapture() {
	emit signalStopCapture();
}

/**
 * Called when start capture is clicked. The specified device files will be opened. Returns false when an error occurs.
 * @param camL
 * @param camR
 * @return bool
 */
bool Ctrl::onClickStartCapture(std::string camL, std::string camR) {

	if(m_grabber.isCaptureing()){
		displayError("Already captureing!");
		return false;
	}

	if (!m_grabber.initGrabber(camL, camR)) {
		displayError("Can't open device!");
		return false;
	}
	emit signalChangeExposureMode(m_gui->getExposureMode());
	emit signalChangeExposureValue(m_gui->getExposureValue());
	emit signalStartCapture();

	m_opMode = OM_FREE_RUN_CAPTURE;
	return true;
}

/**
 * Called when exposure mode has changed.
 * @param mode
 */
void Ctrl::onChangeExposureMode(fcv::ExposureController::ExposureCtrlMode mode) {
	if(m_grabber.isCaptureing())
		emit signalChangeExposureMode(mode);
}

/**
 * Called when exposure value has changed
 * @param val
 */
void Ctrl::onChangeExposureValue(int val) {
	if(m_grabber.isCaptureing())
		emit signalChangeExposureValue(val);
}

/**
 * Called when user want's to save current frame.
 */
void Ctrl::onClickSaveCurrentFrame() {

}

/**
 * Called when user wants to save frame and disparity.
 */
void Ctrl::onClickSaveFrameAndDisp() {

}

/**
 * Called when user wants to save all frame data: images, disparity and point cloud.
 */
void Ctrl::onClickSaveAllFrameData() {

}

/**
 * Called when user wants to capture frame sequence
 */
void Ctrl::onClickCaptureFrameSequenze() {
	if(m_opMode != OM_FREE_RUN_CAPTURE)
	{
		emit displayError("Invalid State!");
		return;
	}

	m_maxSeqNr = 50;
	m_frameDataSequenze.clear();
	m_opMode = OM_CAPTURE_FRAME_SEQUENCE;
	LOG_INFO("Capture sequence started.");
}

/**
 * Called when user wants to process and create a whole sequenze
 */
void Ctrl::onClickProcessFrameSequenze() {

}

void Ctrl::onSwitchTab(Ctrl::tTabName tab) {
	LOG_FORMAT_INFO("Tab changed: %d", (int) tab);

	switch (tab) {
	case TAB_CAPTURE:
//		m_gui->setFrameDataList(m_frameDataSequenze);
		break;
	case TAB_STEREO:
		m_gui->setFrameDataList(m_frameDataSequenze);
		break;
	case TAB_3D:
		m_gui->setFrameDataList(m_frameDataSequenze);
		break;
		default:
			break;
	}
}
void Ctrl::onChangeSelectedFrameData(int id)
{
	if(id >= m_frameDataSequenze.size())
		return;

	StereoFrameData& data = m_frameDataSequenze.at(id);

	m_gui->displayStereoImage(data.imgLeft,data.imgRight, TAB_STEREO);
	m_gui->displayDisparityImage(data.dispLeft,data.dispRight);
	// TODO PC

}
/**
 * Applies undistortion and rectification of new images.
 * @param framelL
 * @param frameR
 */
void Ctrl::preprocessFrames(fcv::Image& framelL, fcv::Image& frameR) {

}
