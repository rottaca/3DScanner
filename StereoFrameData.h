/**
 * Project 3DScanner
 */


#ifndef _STEREOFRAMEDATA_H
#define _STEREOFRAMEDATA_H

#include <string>
#include <vector>

#include <FreeCV/FreeCV.h>

class StereoFrameData {
public: 
    fcv::Image imgLeftRGB;
    fcv::Image imgRightRGB;
    fcv::Image imgLeftGray;
    fcv::Image imgRightGray;
    fcv::Image dispLeft;
    fcv::Image dispRight;
    fcv::PointCloudCreator::PointCloud pointCloudLeft;
    /**
     * contains complete extrinsic transformation from model to world coordinate system.
     */
    fcv::Matrix3x3f cameraPose;
    
    bool ImgValid();
    
    bool DispValid();
    
    bool PointCloudValid();
    
    bool CameraPoseValid();
    
    /**
     * @param frameL
     * @param frameR
     */
    void setFrame(fcv::Image frameLRGB, fcv::Image frameRRGB,fcv::Image frameLGray, fcv::Image frameRGray);
    
    /**
     * @param dispL
     * @param dispR
     */
    void setDisp(fcv::Image dispL, fcv::Image dispR);
    
    /**
     * @param PointCloud pc
     */
    void setPointCloudLeft(fcv::PointCloudCreator::PointCloud pc);
    
    /**
     * @param Matrix3x3f pose
     */
    void setCameraPose(fcv::Matrix3x3f pose);
};

#endif //_STEREOFRAMEDATA_H
