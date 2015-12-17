/**
 * Project 3DScanner
 */


#include "Gui.h"
#include <QtWidgets/qlistwidget.h>

/**
 * Gui implementation
 */

Gui::Gui(Ctrl* ctrl) {
	m_ctrl = ctrl;

	m_form = new Ui::MainForm();
	m_form->setupUi(this);

	connectSignals();

	show();
}
Gui::~Gui() {

}
void Gui::connectSignals()
{
	qRegisterMetaType<fcv::Image>("fcv::Image");
	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<fcv::PointCloudCreator::PointCloud>("fcv::PointCloudCreator::PointCloud");

	// Slots
	connect(m_form->b_start_capture,SIGNAL(clicked()),this,SLOT(onClickStartCapture()));
	connect(m_form->b_captureStereoFrame, SIGNAL(clicked()),this,SLOT(onClickCapture()));
	connect(m_form->tw_view_tabs, SIGNAL(currentChanged(int)),this,SLOT(onChangeTab(int)));
	connect(m_form->li_stereoPairs, SIGNAL(itemSelectionChanged()),this,SLOT(onChangeSelectedFrameData()));

	connect(m_form->sl_exp_manual_ctrl,SIGNAL(valueChanged(int)),this,SLOT(onChangeExposureValue(int)));
	connect(m_form->sl_exp_manual_fixed,SIGNAL(valueChanged(int)),this,SLOT(onChangeExposureValue(int)));

	connect(m_form->rb_exp_auto, SIGNAL(clicked()),this,SLOT(onChangeExposureMode()));
	connect(m_form->rb_exp_manual_ctrl, SIGNAL(clicked()),this,SLOT(onChangeExposureMode()));
	connect(m_form->rb_exp_manual_fixed, SIGNAL(clicked()),this,SLOT(onChangeExposureMode()));


	connect(m_form->b_process_seq, SIGNAL(clicked()), this, SLOT(onClickProcessFrameSequence()));

//	connect(&m_hwCtrl,SIGNAL(signalRotated(float)),this,SLOT(receivePlatformRotated(float)));
//	connect(&m_hwCtrl,SIGNAL(signalError(std::string)),this,SLOT(displayError(std::string)));
//
//	connect(&m_pcProc,SIGNAL(signalPointCloud(fcv::PointCloudCreator::PointCloud)),this,SLOT(receivePointCloud(fcv::PointCloudCreator::PointCloud)));
//	connect(&m_pcProc,SIGNAL(signalError(std::string)),this,SLOT(displayError(std::string)));
//
//	connect(&m_stereoProc,SIGNAL(signalDisparity(fcv::Image, fcv::Image)),this,SLOT(receiveDispairty(fcv::Image, fcv::Image)));
//	connect(&m_stereoProc,SIGNAL(signalError(std::string)),this,SLOT(displayError(std::string)));
//
//	// Signals
//	connect(this,SIGNAL(signalStartCapture()),&m_grabber,SLOT(startCapture()));
//	connect(this,SIGNAL(signalStopCapture()),&m_grabber,SLOT(stopCapture()));
//	connect(this,SIGNAL(signalStartCapture()),&m_grabber,SLOT(startCapture()));
//	connect(this,SIGNAL(signalStopCapture()),&m_grabber,SLOT(stopCapture()));


}
/**
 * Displays the two stereo images in the gui.
 * @param left
 * @param right
 */
void Gui::displayStereoImage(fcv::Image left, fcv::Image right, Ctrl::tTabName tab) {
	switch (tab) {
	case Ctrl::TAB_CAPTURE:
		m_form->l_img_left_capture->setPixmap(QPixmap::fromImage(left.toQImage()));
		m_form->l_img_right_capture->setPixmap(QPixmap::fromImage(right.toQImage()));
		break;
	case Ctrl::TAB_STEREO:
		m_form->l_img_left_proc->setPixmap(QPixmap::fromImage(left.toQImage()));
		m_form->l_img_right_proc->setPixmap(QPixmap::fromImage(right.toQImage()));
		break;
	case Ctrl::TAB_3D:

		break;
	default:
		break;
	}
}

/**
 * Displays the two disparity images.
 * @param left
 * @param right
 */
void Gui::displayDisparityImage(fcv::Image left, fcv::Image right) {
	if(!left.isValid() ||!right.isValid())
		return;
	fcv::Image dispLColored;
	fcv::Image dispRColored;
	float maxRealDisp0 = *std::max_element(left.getPtr<float>(), (float*) (left.getPtr<float>() + left.getWidth() * left.getHeight()));
	float maxRealDisp1 = *std::max_element(right.getPtr<float>(), (float*) (right.getPtr<float>() + right.getWidth() * right.getHeight()));
	float maxRealDisp = maxRealDisp0> maxRealDisp1? maxRealDisp0:maxRealDisp1;

	fcv::convertToPseudoColor(&left, &dispLColored, 0, maxRealDisp, 0, 120);
	fcv::convertToPseudoColor(&right, &dispRColored, 0, maxRealDisp, 0, 120);

	m_form->l_disp_left_proc->setPixmap(QPixmap::fromImage(dispLColored.toQImage()));
	m_form->l_disp_right_proc->setPixmap(QPixmap::fromImage(dispRColored.toQImage()));
}

/**
 * adds a pointcloud for display purposes to the scene.
 * @param pc
 * @return int
 */
int Gui::addPointCloud(fcv::PointCloudCreator::PointCloud pc) {
    return 0;
}

/**
 * reomoves all displayed point clouds
 */
void Gui::removePointClouds() {

}

/**
 * returns the current exposure mode
 * @return ExposureMode
 */
fcv::ExposureController::ExposureCtrlMode Gui::getExposureMode() {
	if(m_form->rb_exp_auto->isChecked())
		return fcv::ExposureController::AUTOMATIC;
	else if(m_form->rb_exp_manual_ctrl->isChecked())
		return fcv::ExposureController::MANUAL_CTRL;
	else
		return fcv::ExposureController::MANUAL_FIXED;
}

/**
 * returns the current exposure value
 * @return int
 */
int Gui::getExposureValue() {
	if(m_form->rb_exp_manual_ctrl->isChecked())
		return m_form->sl_exp_manual_ctrl->value();
	else if(m_form->rb_exp_manual_fixed->isChecked())
		return m_form->sl_exp_manual_fixed->value();
	else
		return -1;
}

/**
 * slot for starting capture click
 */
void Gui::onClickStartCapture() {
	if(m_form->b_start_capture->text().compare("Start Capture") == 0){
		if(m_ctrl->onClickStartCapture(m_form->cb_left_cam->currentText().toStdString(),m_form->cb_right_cam->currentText().toStdString())){
			m_form->b_start_capture->setText("Stop Capture");
		}
	}
	else{
		m_ctrl->onClickStopCapture();
		m_form->b_start_capture->setText("Start Capture");
	}
}

void Gui::onClickCapture() {
	m_ctrl->onClickCaptureFrameSequenze();
}

/**
 * slot for changing exposure mode
 */
void Gui::onChangeExposureMode() {

	if(m_form->rb_exp_auto->isChecked())
		m_ctrl->onChangeExposureMode(fcv::ExposureController::AUTOMATIC);
	else if(m_form->rb_exp_manual_ctrl->isChecked())
		m_ctrl->onChangeExposureMode(fcv::ExposureController::MANUAL_CTRL);
	else
		m_ctrl->onChangeExposureMode(fcv::ExposureController::MANUAL_FIXED);
}

/**
 * slot for changing exposure value
 * @param val
 */
void Gui::onChangeExposureValue(int val) {
	if(m_form->rb_exp_manual_ctrl->isChecked())
		m_ctrl->onChangeExposureValue(m_form->sl_exp_manual_ctrl->value());
	else if(m_form->rb_exp_manual_fixed->isChecked())
		m_ctrl->onChangeExposureValue(m_form->sl_exp_manual_fixed->value());
}
void Gui::onChangeTab(int idx)
{
	switch (idx) {
	case 0:
		m_ctrl->onSwitchTab(Ctrl::TAB_CAPTURE);
		break;
	case 1:
		m_ctrl->onSwitchTab(Ctrl::TAB_STEREO);
		break;
	case 2:
		m_ctrl->onSwitchTab(Ctrl::TAB_3D);
		break;
	default:
		break;
	}
}
void Gui::setFrameDataList(std::vector<StereoFrameData>& frameDataSequenze)
{
	m_form->li_stereoPairs->blockSignals(true);
	m_form->li_stereoPairs->clear();
	for (int i = 0; i < frameDataSequenze.size(); i++) {
		QListWidgetItem *newItem = new MyQListWidgetItem(i, frameDataSequenze.at(i).DispValid(),frameDataSequenze.at(i).PointCloudValid());

		m_form->li_stereoPairs->insertItem(m_form->li_stereoPairs->count(), newItem);
	}
	m_form->li_stereoPairs->blockSignals(false);

}
void Gui::onChangeSelectedFrameData()
{

	MyQListWidgetItem* item = (MyQListWidgetItem*)m_form->li_stereoPairs->selectedItems().at(0);
//	m_form->b_process_curr_frame->setEnabled(!item->getDispDone());
	LOG_FORMAT_INFO("Clicked on %d",(item->getId()));
	m_ctrl->onChangeSelectedFrameData(item->getId());
}

void Gui::onClickProcessFrameSequence()
{
	m_ctrl->onClickProcessFrameSequenze();
}
