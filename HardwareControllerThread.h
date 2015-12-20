/**
 * Project 3DScanner
 */


#ifndef _HARDWARECONTROLLERTHREAD_H
#define _HARDWARECONTROLLERTHREAD_H


#include <QtWidgets/qwidget.h>
#include <QtCore/qthread.h>

#include <string>
#include <vector>

#include "HardwareController.h"

class HardwareControllerThread: public QObject {
	Q_OBJECT
public: 
    
    HardwareControllerThread();
    ~HardwareControllerThread();

public slots:
    /**
     * Sets the angle of the platform. A signal is emitted when the platform reaches the specified angle.
     * @param angle
     */
    void setPlatformAngle(float angle);
    
    /**
     * initializes the platform controller and opens a connection to the specified device.
     * @param device
     */
    bool initPlatformController(std::string device);
signals:
    /**
     * @param errStr
     */
    void signalError(std::string errStr);
    
    /**
     * @param angle
     */
    void signalRotated(float angle);
private: 
    HardwareController m_hwCtrl;
    QThread m_thread;
};

#endif //_HARDWARECONTROLLERTHREAD_H
