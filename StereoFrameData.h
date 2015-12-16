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
    fcv::Image imgLeft;
    fcv::Image imgRight;
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
    void setFrame(fcv::Image frameL, fcv::Image frameR);
    
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
