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
    void initPCC(fcv::Vector2f C, float f, float b);
    
    /**
     * @param dispL
     * @param frameL
     */
    void startProcessing(fcv::Image disp, fcv::Image img, fcv::Matrix4x4f pose, int id);
    
signals:
    /**
     * @param errStr
     */
    void signalError(std::string errStr);
    
    /**
     * @param pcL
     */
    void signalPointCloud(fcv::PointCloudCreator::PointCloud pcL, int id);
private: 
    fcv::PointCloudCreator m_pcc;
    QThread m_thread;
    fcv::Vector2f m_C;
    float m_f;
    float m_b;
};

#endif //_PCPROCESSINGTHREAD_H
