/**
 * Project 3DScanner
 */


#include "HardwareController.h"
#include <QtCore/qthread.h>
/**
 * HardwareController implementation
 */


/**
 * @param angle
 */
bool HardwareController::setPlatformAngle(float angle) {
	// Simulate movement
	QThread::sleep(5);
	return true;
}

/**
 * @return float
 */
float HardwareController::getPlatformAngle() {
    return 0.0;
}

/**
 * @param device
 * @return bool
 */
bool HardwareController::initPlatformController(std::string device) {
    return false;
}

