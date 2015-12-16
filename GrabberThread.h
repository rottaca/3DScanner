/**
 * Project 3DScanner
 */


#ifndef _GRABBERTHREAD_H
#define _GRABBERTHREAD_H

#include <QtWidgets/qwidget.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>

#include <string>
#include <vector>

#include <FreeCV/FreeCV.h>

class GrabberThread: public QObject {
	Q_OBJECT
public: 
	GrabberThread();
	~GrabberThread();
    /**
     * Stores the device names for the cams in member variables. But does no Initialization.
     * 
     * @param camL
     * @param camR
     */
    bool initGrabber(std::string camL, std::string camR);
    
    bool isCaptureing(){
    	return m_isCaptureing;
    }

public slots:
    /**
     * Stops the grabbing and closes the device files.
     */
    void stopCapture();
    
    /**
     * Opens and initializes the driver and starts grabbing.
     */
    void startCapture();
    
    /**
     * Switches between exposure modes: manual (fixed exp), manual (fixed goal value, automatic ctrl), automatic mode (use camera own exposure).
     * @param mode
     */
    void changeExposureMode(fcv::ExposureController::ExposureCtrlMode mode);
    
    /**
     * Adjusts the exposure value according to the exposure mode.
     * @param val
     */
    void changeExposureValue(int val);
    
    void grabFrame();

signals:
    /**
     * @param frameL
     * @param frameR
     */
    void signalNewFrame(fcv::Image frameL, fcv::Image frameR);
    
    /**
     * @param errStr
     */
    void signalError(std::string errStr);

private: 
    fcv::VideoCapture m_grabber1, m_grabber2;
	fcv::ExposureController m_exposureCtrl1,m_exposureCtrl2;
    QThread m_thread;
    QTimer m_timer;

    bool m_isCaptureing;
};

#endif //_GRABBERTHREAD_H
