/***************************************************************************
                          addconstraintactivitiespreferredtimeslotsform.h  -  description
                             -------------------
    begin                : 15 May 2004
    copyright            : (C) 2004 by Liviu Lalescu
    email                : Please see https://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find there the email address)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#ifndef ADDCONSTRAINTACTIVITIESPREFERREDTIMESLOTSFORM_H
#define ADDCONSTRAINTACTIVITIESPREFERREDTIMESLOTSFORM_H

#include "ui_addconstraintactivitiespreferredtimeslotsform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintActivitiesPreferredTimeSlotsForm : public QDialog, Ui::AddConstraintActivitiesPreferredTimeSlotsForm_template  {
	Q_OBJECT

public:
	AddConstraintActivitiesPreferredTimeSlotsForm(QWidget* parent);
	~AddConstraintActivitiesPreferredTimeSlotsForm();

	void updateTeachersComboBox();
	void updateStudentsComboBox();
	void updateSubjectsComboBox();
	void updateActivityTagsComboBox();

	void colorItem(QTableWidgetItem* item);

public slots:
	void addConstraint();
	
	void itemClicked(QTableWidgetItem* item);
	void horizontalHeaderClicked(int col);
	void verticalHeaderClicked(int row);
	
	void cellEntered(int row, int col);
	
	void setAllSlotsAllowed();
	void setAllSlotsNotAllowed();

	void durationCheckBox_toggled();
};

#endif
