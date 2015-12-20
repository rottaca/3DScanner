/**
 * Project 3DScanner
 */


#include "HardwareControllerThread.h"

/**
 * HardwareControllerThread implementation
 * 
 * Controls the Hardware that rotates the platform.
 */

HardwareControllerThread::HardwareControllerThread() {
	m_thread.setParent(this);
	moveToThread(&m_thread);
	m_thread.start();
}
HardwareControllerThread::~HardwareControllerThread() {

}

/**
 * Sets the angle of the platform. A signal is emitted when the platform reaches the specified angle.
 * @param angle
 */
void HardwareControllerThread::setPlatformAngle(float angle) {
	if(m_hwCtrl.setPlatformAngle(angle))
		emit signalRotated(angle);
	else
		emit signalError("Platform rotation failed!");
}


/**
 * initializes the platform controller and opens a connection to the specified device.
 * @param device
 * @return bool
 */
bool HardwareControllerThread::initPlatformController(std::string device) {
    return false;
}

