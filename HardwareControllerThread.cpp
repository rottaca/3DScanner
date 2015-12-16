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

}

/**
 * returns the current angle (assuming that the last rotation command is completely executed)
 * @return float
 */
float HardwareControllerThread::getPlatformAngle() {
    return 0.0;
}

/**
 * initializes the platform controller and opens a connection to the specified device.
 * @param device
 * @return bool
 */
bool HardwareControllerThread::initPlatformController(std::string device) {
    return false;
}

