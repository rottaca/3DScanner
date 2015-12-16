/**
 * Project 3DScanner
 */


#ifndef _POINTCLOUD_H
#define _POINTCLOUD_H

#include "Drawable.h"

#include <FreeCV/FreeCV.h>

class DrawablePointCloud: public Drawable {
public: 
    int m_vaoId;
    int m_vboId;
    fcv::PointCloudCreator::PointCloud m_pointCloud;
    
    void createBuffers();
    
    void drawData();
    // TODO
//    mat4 getModelMatrix();
    
    /**
     * @param pc
     */
    void setPointCloudData(fcv::PointCloudCreator::PointCloud pc);
};

#endif //_POINTCLOUD_H
