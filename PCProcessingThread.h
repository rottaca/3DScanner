/**
 * Project 3DScanner
 */


#ifndef _PCPROCESSINGTHREAD_H
#define _PCPROCESSINGTHREAD_H

#include <QtWidgets/qwidget.h>
#include <QtCore/qthread.h>

#include <string>
#include <vector>

#include <FreeCV/FreeCV.h>

class PCProcessingThread: public QObject {
	Q_OBJECT
public: 
	PCProcessingThread();
	~PCProcessingThread();
public slots:
    /**
     * @param ...
     */
    void initPCC();
    
    /**
     * @param dispL
     * @param frameL
     */
    void startProcessing(fcv::Image dispL, fcv::Image frameL);
    
signals:
    /**
     * @param errStr
     */
    void signalError(std::string errStr);
    
    /**
     * @param pcL
     */
    void signalPointCloud(fcv::PointCloudCreator::PointCloud pcL);
private: 
    fcv::PointCloudCreator m_pcc;
    QThread m_thread;
};

#endif //_PCPROCESSINGTHREAD_H
