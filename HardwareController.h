/**
 * Project 3DScanner
 */


#ifndef _HARDWARECONTROLLER_H
#define _HARDWARECONTROLLER_H

#include <string>
#include <vector>

class HardwareController{
public: 
    
    /**
     * @param angle
     */
    bool setPlatformAngle(float angle);
    
    float getPlatformAngle();
    
    /**
     * @param device
     */
    bool initPlatformController(std::string device);

private: 
    float m_currentAngle;
};

#endif //_HARDWARECONTROLLER_H
