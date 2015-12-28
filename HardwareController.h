/**
 * Project 3DScanner
 */


#ifndef _HARDWARECONTROLLER_H
#define _HARDWARECONTROLLER_H

#include <string>
#include <vector>

#include <QtSerialPort/qserialport.h>
#include <QtCore/qobject.h>

#include <FreeCV/FreeCV.h>

class HardwareController: public QObject{
	Q_OBJECT

public: 
	HardwareController();
	~HardwareController();
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
    QSerialPort* m_port;
};

#endif //_HARDWARECONTROLLER_H
