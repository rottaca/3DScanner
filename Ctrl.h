/**
 * Project 3DScanner
 */


#ifndef _CTRL_H
#define _CTRL_H

#include <QtWidgets/qwidget.h>

#include "GrabberThread.h"
#include "StereoProcessingThread.h"
#include "PCProcessingThread.h"
#include "StereoFrameData.h"
#include "HardwareControllerThread.h"
#include "OperationMode.h"

#include <string>
#include <vector>

#include <FreeCV/FreeCV.h>

#include <opencv2/opencv.hpp>

class Gui;

class Ctrl: public QObject {
	Q_OBJECT
public: 

	Ctrl();
	~Ctrl();
	/**
	 * Called when stop capture is clicked
	 */
	void onClickStopCapture();

	/**
	 * Called when start capture is clicked. The specified device files will be opened. Returns false when an error occurs.
	 * @param camL
	 * @param camR
	 */
	bool onClickStartCapture(std::string camL, std::string camR);

	/**
	 * Called when exposure mode has changed.
	 * @param mode
	 */
	void onChangeExposureMode(fcv::ExposureController::ExposureCtrlMode mode);

	/**
	 * Called when exposure value has changed
	 * @param val
	 */
	void onChangeExposureValue(int val);

	/**
	 * Called when user want's to save current frame.
	 */
	void onClickSaveCurrentFrame();

	/**
	 * Called when user wants to save frame and disparity.
	 */
	void onClickSaveFrameAndDisp();

	/**
	 * Called when user wants to save all frame data: images, disparity and point cloud.
	 */
	void onClickSaveAllFrameData();

	/**
	 * Called when user wants to capture frame sequence
	 */
	void onClickCaptureFrameSequenze();

	/**
	 * Called when user wants to process and create a whole sequence
	 */
	void onClickProcessFrameSequenze();

	void onClickProcessPC();

	enum tTabName {TAB_CAPTURE, TAB_STEREO, TAB_3D};
	void onSwitchTab(tTabName tab);
	void onChangeSelectedFrameData(int id);

signals:
	void signalStartCapture();
	void signalStopCapture();
	void signalChangeExposureMode(fcv::ExposureController::ExposureCtrlMode mode);
	void signalChangeExposureValue(int val);
	void signalProcessStereoFrame(fcv::Image imgL, fcv::Image imgR, int id);
	void signalProcessPC(fcv::Image disp, fcv::Image img, fcv::Matrix4x4f pose, int id);
	void signalRotatePlatform(float goalAngle);

public slots:
    /**
     * Displays an error message
     * @param errStr
     */
    void displayError(std::string errStr);
    
    /**
     * slot for receiving the new stereo image pair
     * @param frameL
     * @param frameR
     */
    void receiveNewFrame(fcv::Image frameL, fcv::Image frameR);
    
    /**
     * slot for receiving next disparity image pair
     * @param dispL
     * @param dispR
     */
    void receiveDispairty(fcv::Image dispL, fcv::Image dispR, int id);
    
    /**
     * slot for receiving the new point cloud
     * @param pcL
     */
    void receivePointCloud(fcv::PointCloudCreator::PointCloud pcL, int id);
    
    /**
     * slot that is called when hardware platform rotation is executed completely.
     * @param angle
     */
    void receivePlatformRotated(float angle);
    
private: 
    /**
     * Contains undistort rectify map for openCV.
     */
    cv::Mat m_undistortMapXL;
    /**
     * Contains undistort rectify map for openCV.
     */
    cv::Mat m_undistortMapXR;
    /**
     * Contains undistort rectify map for openCV.
     */
    cv::Mat m_undistortMapYL;
    /**
     * Contains undistort rectify map for openCV.
     */
    cv::Mat m_undistortMapYR;

    /**
     * Contains the maximum frame index.
     */
    int m_maxSeqNr;
    Gui* m_gui;
    GrabberThread m_grabber;
    StereoProcessingThread m_stereoProc;
    PCProcessingThread m_pcProc;
    std::vector<StereoFrameData> m_frameDataSequenze;
    HardwareControllerThread m_hwCtrl;
    OperationMode m_opMode;
    
	cv::Mat map1x, map1y, map2x, map2y;
	cv::Mat D1, D2;
	cv::Mat CM1;
	cv::Mat CM2;
	cv::Mat R1, R2, P1, P2;
	cv::Mat T, H_world_cam;

	int m_platformRotSubDevCnt;
	int m_currPlatformRotNr;
	bool m_platformMoving;

    /**
     * Applies undistortion and rectification of new images.
     * @param framelL
     * @param frameR
     */
    void preprocessFrames(fcv::Image& framelL, fcv::Image& frameR);

    void connectSignals();
    /**
     * Loads all camera calibration files.
     */
    void loadCalibrationFiles();
};

#endif //_CTRL_H
