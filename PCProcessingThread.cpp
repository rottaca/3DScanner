/**
 * Project 3DScanner
 */


#include "PCProcessingThread.h"

/**
 * PCProcessingThread implementation
 */

PCProcessingThread::PCProcessingThread() {
	m_thread.setParent(this);
	moveToThread(&m_thread);
	m_thread.start();
}
PCProcessingThread::~PCProcessingThread() {

}
/**
 * @param ...
 */
void PCProcessingThread::initPCC() {

}

/**
 * @param dispL
 * @param frameL
 */
void PCProcessingThread::startProcessing(fcv::Image dispL, fcv::Image frameL) {

}
