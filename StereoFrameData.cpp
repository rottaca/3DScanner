/**
 * Project 3DScanner
 */


#include "StereoFrameData.h"

/**
 * StereoFrameData implementation
 */


/**
 * @return bool
 */
bool StereoFrameData::ImgValid() {
    return imgLeftRGB.isValid() && imgLeftRGB.isValid() && imgLeftGray.isValid() && imgRightGray.isValid();
}

/**
 * @return bool
 */
bool StereoFrameData::DispValid() {
    return dispLeft.isValid() && dispRight.isValid();
}

/**
 * @return bool
 */
bool StereoFrameData::PointCloudValid() {
    return pointCloudLeft.size() > 0;
}

/**
 * @return bool
 */
bool StereoFrameData::CameraPoseValid() {
    return !cameraPose.isEmpty();
}

/**
 * @param frameL
 * @param frameR
 */
void StereoFrameData::setFrame(fcv::Image frameLRGB, fcv::Image frameRRGB,fcv::Image frameLGray, fcv::Image frameRGray) {
	imgLeftRGB = frameLRGB;
	imgRightRGB = frameRRGB;
	imgLeftGray = frameLGray;
	imgRightGray = frameRGray;
}

/**
 * @param dispL
 * @param dispR
 */
void StereoFrameData::setDisp(fcv::Image dispL, fcv::Image dispR) {
	dispLeft = dispL;
	dispRight = dispR;

}

/**
 * @param PointCloud pc
 */
void StereoFrameData::setPointCloudLeft(fcv::PointCloudCreator::PointCloud pc) {
	pointCloudLeft = pc;
}

/**
 * @param Matrix3x3f pose
 */
void StereoFrameData::setCameraPose(fcv::Matrix4x4f pose) {
	cameraPose = pose;
}
