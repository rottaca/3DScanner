/**
 * Project 3DScanner
 */


#ifndef _POINTCLOUD_H
#define _POINTCLOUD_H

#include "Drawable.h"

#include <FreeCV/FreeCV.h>

class DrawablePointCloud: public Drawable {
public: 
	DrawablePointCloud();
	~DrawablePointCloud();

    bool createBuffers(int attrPos, int attrCol);
    
    void drawData();
    // TODO
//    mat4 getModelMatrix();
    
    /**
     * @param pc
     */
    void setPointCloudData(fcv::PointCloudCreator::PointCloud pc, float pointSize);

private:
    fcv::PointCloudCreator::PointCloud m_pointCloud;
    float m_pointSize;

};

#endif //_POINTCLOUD_H
