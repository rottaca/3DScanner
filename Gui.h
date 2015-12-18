/**
 * Project 3DScanner
 */


#ifndef _GUI_H
#define _GUI_H

#include <QtWidgets/qwidget.h>
#include <QtCore/qsettings.h>

#include "Ctrl.h"
#include "GLScene.h"
#include "ui_mainform.h"
#include "StereoFrameData.h"
#include "MyQListWidgetItem.h"

#include <string>
#include <vector>

#include <FreeCV/FreeCV.h>


class Gui: public QWidget {
	Q_OBJECT
public: 
    
	Gui(Ctrl* ctrl);
	~Gui();
    
    /**
     * adds a pointcloud for display purposes to the scene.
     * @param pc
     */
    int addPointCloud(fcv::PointCloudCreator::PointCloud pc);
    
    /**
     * reomoves all displayed point clouds
     */
    void removePointClouds();
    
    /**
     * returns the current exposure mode
     */
    fcv::ExposureController::ExposureCtrlMode getExposureMode();
    
    /**
     * returns the current exposure value
     */
    int getExposureValue();
    /**
     * Displays the two stereo images in the gui.
     * @param left
     * @param right
     */
    void displayStereoImage(fcv::Image left, fcv::Image right, Ctrl::tTabName tab);
    
    /**
     * Displays the two disparity images.
     * @param left
     * @param right
     */
    void displayDisparityImage(fcv::Image left, fcv::Image right);

    void setFrameDataList(std::vector<StereoFrameData>& frameDataSequenze);

    int getMaxDisp(){return m_form->sb_maxDisp->value();}
    int getP1(){return m_form->sb_penalty1->value();}
    int getP2(){return m_form->sb_penalty2->value();}

public slots:
    /**
     * slot for starting capture click
     */
    void onClickStartCapture();

    /**
     * slot for click on capture frames
     */
    void onClickCapture();

    /**
     * slot for changing exposure mode
     */
    void onChangeExposureMode();
    
    /**
     * slot for changing exposure value
     * @param val
     */
    void onChangeExposureValue(int val);

    void onChangeTab(int idx);
    void onChangeSelectedFrameData();

    void onClickProcessFrameSequence();
    void onClickProcessPC();

private: 
    void connectSignals();

    QSettings m_guiSettings;
    GLScene* m_glScene;

	Ui::MainForm* m_form;
    Ctrl* m_ctrl;
};

#endif //_GUI_H
