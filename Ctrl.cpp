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
	loadCalibrationFiles();
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

	connect(&m_stereoProc, SIGNAL(signalDisparity(fcv::Image, fcv::Image, int)), this, SLOT(receiveDispairty(fcv::Image, fcv::Image, int)));
	connect(&m_stereoProc, SIGNAL(signalError(std::string)), this, SLOT(displayError(std::string)));

	// Signals
	connect(this, SIGNAL(signalStartCapture()), &m_grabber, SLOT(startCapture()));
	connect(this, SIGNAL(signalStopCapture()), &m_grabber, SLOT(stopCapture()));

	connect(this, SIGNAL(signalProcessStereoFrame(fcv::Image, fcv::Image, int)), &m_stereoProc, SLOT(startProcessing(fcv::Image, fcv::Image, int)));

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
	cv::FileStorage fs1("stereoCalib.yml", cv::FileStorage::READ);
	cv::FileStorage fs2("extrinsicCameraCalib.yml", cv::FileStorage::READ);
	if (!fs1.isOpened()) {
		displayError("Can't find stereoCalib.yml");
		exit(-1);
	}
	if (!fs2.isOpened()) {
		displayError("Can't find extrinsicCameraCalib.yml");
		exit(-1);
	}
	fs1["CM1"] >> CM1;
	fs1["CM2"] >> CM2;
	fs1["R1"] >> R1;
	fs1["R2"] >> R2;
	fs1["P1"] >> P1;
	fs1["P2"] >> P2;
	fs1["D1"] >> D1;
	fs1["D2"] >> D2;
	fs1["T"] >> T;
	fs2["InvExtTransform"] >> H_world_cam;
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

		if(m_frameDataSequenze.size() >= m_maxSeqNr){
			m_gui->setFrameDataList(m_frameDataSequenze);
			m_opMode = OM_FREE_RUN_CAPTURE;
		}

		LOG_FORMAT_INFO("Added frame %d of %d",m_frameDataSequenze.size(),m_maxSeqNr);
	}
}

/**
 * slot for receiving next disparity image pair
 * @param dispL
 * @param dispR
 */
void Ctrl::receiveDispairty(fcv::Image dispL, fcv::Image dispR, int id) {
	LOG_FORMAT_INFO("Disparity received with id %d", id);

	if(id < m_frameDataSequenze.size())
		m_frameDataSequenze.at(id).setDisp(dispL, dispR);

	// Update
	m_gui->setFrameDataList(m_frameDataSequenze);

	if(++id < m_frameDataSequenze.size())
	{
		fcv::Image imgLGray, imgRGray;
		fcv::convertPxFormat(&m_frameDataSequenze.at(id).imgLeft, &imgLGray, fcv::RGB_888_TO_GRAY8);
		fcv::convertPxFormat(&m_frameDataSequenze.at(id).imgRight, &imgRGray, fcv::RGB_888_TO_GRAY8);
		emit signalProcessStereoFrame(imgLGray, imgRGray, id);
	}else{
		if(m_grabber.isCaptureing())
			m_opMode = OM_FREE_RUN_CAPTURE;
		else
			m_opMode = OM_IDLE;
	}

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

	if(m_opMode == OM_FREE_RUN_CAPTURE)
		m_opMode = OM_IDLE;
}

/**
 * Called when start capture is clicked. The specified device files will be opened. Returns false when an error occurs.
 * @param camL
 * @param camR
 * @return bool
 */
bool Ctrl::onClickStartCapture(std::string camL, std::string camR) {
	if (m_opMode != OM_IDLE) {
		displayError("Invalid state!");
		return false;
	}

	if(m_grabber.isCaptureing()){
		displayError("Already captureing!");
		return false;
	}

	if (!m_grabber.initGrabber(camL, camR)) {
		displayError("Can't open device!");
		return false;
	}

	initUndistortRectifyMap(CM1, D1, R1, P1, cv::Size(m_grabber.getGrabberL()->getWidth(),m_grabber.getGrabberL()->getHeight()), CV_32FC1, map1x, map1y);
	initUndistortRectifyMap(CM2, D2, R2, P2, cv::Size(m_grabber.getGrabberL()->getWidth(),m_grabber.getGrabberL()->getHeight()), CV_32FC1, map2x, map2y);

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

	m_maxSeqNr = 1;
	m_frameDataSequenze.clear();
	m_opMode = OM_CAPTURE_FRAME_SEQUENCE;
	LOG_INFO("Capture sequence started.");
}

/**
 * Called when user wants to process and create a whole sequenze
 */
void Ctrl::onClickProcessFrameSequenze() {
	if(m_frameDataSequenze.size()<= 0){
		displayError("No frame data available!");
		return;
	}
	else if(m_opMode != OM_FREE_RUN_CAPTURE && m_opMode != OM_IDLE){
		displayError("Invalid state!");
		return;
	}

	fcv::Image imgLGray, imgRGray;
	fcv::convertPxFormat(&m_frameDataSequenze.at(0).imgLeft,&imgLGray,fcv::RGB_888_TO_GRAY8);
	fcv::convertPxFormat(&m_frameDataSequenze.at(0).imgRight,&imgRGray,fcv::RGB_888_TO_GRAY8);
	int disp = m_gui->getMaxDisp();
	int p1 = m_gui->getP1();
	int p2 = m_gui->getP2();
	if(p2 < p1)
	{
		displayError("Penalty 2 has to be >= penalty 1");
		return;
	}
	m_stereoProc.initSGM(imgLGray.getWidth(),imgLGray.getHeight(),disp,p1,p2);

	m_opMode = OM_PROCESSING_STEREO;
	emit signalProcessStereoFrame(imgLGray,imgRGray,0);
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
//		m_gui->setFrameDataList(m_frameDataSequenze);
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

	cv::Mat img1(cv::Size(framelL.getWidth(),framelL.getHeight()), CV_8UC3, framelL.getPtr<unsigned char>());
	cv::Mat img2(cv::Size(frameR.getWidth(),frameR.getHeight()), CV_8UC3, frameR.getPtr<unsigned char>());

    cv::remap(img1, img1, map1x, map1y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
    cv::remap(img2, img2, map2x, map2y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
}
