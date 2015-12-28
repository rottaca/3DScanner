/**
 * Project 3DScanner
 */


#include "HardwareController.h"
#include <QtCore/qthread.h>
/**
 * HardwareController implementation
 */

HardwareController::HardwareController()
{
	m_port = new QSerialPort(this);
	m_currentAngle = 0;
}
HardwareController::~HardwareController()
{
	delete m_port;
}

/**
 * @param angle
 */
bool HardwareController::setPlatformAngle(float angle) {
	// Simulate movement
	std::cout << "Move to Angle: " << angle << std::endl;
	QThread::sleep(5);
	std::cout << "Position reached" << std::endl;
	m_currentAngle = angle;
//	QString data("GOTO: %1");
//	data.arg(angle);
//
//	int numWrite = m_port->write(data.toLocal8Bit());
//	if(numWrite != data.length())
//	{
//		LOG_ERROR("Can't send all data");
//		return false;
//	}
//
//	long int time = fcv::StopWatch::getCurrentTime();
//
//	int numRead;
//	char buff[50];
//	// Timeout after 1 sec
//	while(fcv::StopWatch::getCurrentTime() - time > 1000000){
//		numRead = m_port->readLine(buff,50);
//
//		if(numRead == 0)
//			continue;
//		else if(numRead < 0)
//		{
//			LOG_FORMAT_ERROR("%s", m_port->errorString().toStdString().c_str());
//			return false;
//		}
//
//		QString str(buff);
//		if(str.compare("REACHED")){
//			return true;
//		}
//	}
//
//	return false;


	return true;
}

/**
 * @return float
 */
float HardwareController::getPlatformAngle() {
    return m_currentAngle;
}

/**
 * @param device
 * @return bool
 */
bool HardwareController::initPlatformController(std::string device) {
	m_port->setPortName(device.c_str());
	m_port->setBaudRate(QSerialPort::Baud9600);

	if(!m_port->open(QIODevice::ReadWrite)){
		LOG_FORMAT_ERROR("%s", m_port->errorString().toStdString().c_str());
		return false;
	}

    return true;
}

