/*
 * MyQListWidgetItem.h
 *
 *  Created on: 16.12.2015
 *      Author: andreas
 */

#ifndef MY_Q_WIDGETLIST_ITEM_
#define MY_Q_WIDGETLIST_ITEM_

#include <QtWidgets/qlistwidget.h>

class MyQListWidgetItem : public QListWidgetItem {
public:
	MyQListWidgetItem(int id, bool dispDone, bool pcDone);
	virtual ~MyQListWidgetItem();

	int getId(){return m_id;}
	bool getDispDone(){return m_dispDone;}
	bool getPCDone(){return m_pcDone;}

private:
	int m_id;
	bool m_dispDone;
	bool m_pcDone;
};

#endif /* MY_Q_WIDGETLIST_ITEM_ */
