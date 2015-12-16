/**
 * Project 3DScanner
 */


#ifndef _STEREOPROCESSINGTHREAD_H
#define _STEREOPROCESSINGTHREAD_H

#include <QtWidgets/qwidget.h>
#include <QtCore/qthread.h>

#include <string>
#include <vector>

#include <FreeCV/FreeCV.h>


class StereoProcessingThread: public QObject {
	Q_OBJECT
public: 
    
	StereoProcessingThread();
	~StereoProcessingThread();

public slots:
    /**
     * @param ...
     */
    void initSGM();
    
    /**
     * Starts proceesing and calculates disparity maps.
     * @param frameL
     * @param frameR
     */
    void startProcessing(fcv::Image frameL, fcv::Image frameR);
    
signals:
    /**
     * @param dispL
     * @param dispR
     */
    void signalDisparity(fcv::Image dispL, fcv::Image dispR);
    
    /**
     * @param errStr
     */
    void signalError(std::string errStr);
private: 
    fcv::SGM m_sgm;
    QThread m_thread;
};

#endif //_STEREOPROCESSINGTHREAD_H
