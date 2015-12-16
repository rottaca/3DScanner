/*
 * MyQListWidgetItem.cpp
 *
 *  Created on: 16.12.2015
 *      Author: andreas
 */

#include "MyQListWidgetItem.h"

MyQListWidgetItem::MyQListWidgetItem(int id, bool dispDone, bool pcDone) {
	setText(QString("No: %1 D: %2 | PC: %3").arg(id).arg(dispDone?"valid":"missing",pcDone?"valid":"missing"));
	m_id = id;
	m_dispDone = dispDone;
	m_pcDone = pcDone;
}

MyQListWidgetItem::~MyQListWidgetItem() {
	// TODO Auto-generated destructor stub
}

