/***************************************************************************
                          modifyconstraintactivitiespreferredstartingtimesform.h  -  description
                             -------------------
    begin                : 11 Feb 2005
    copyright            : (C) 2005 by Liviu Lalescu
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

#ifndef MODIFYCONSTRAINTACTIVITIESPREFERREDSTARTINGTIMESFORM_H
#define MODIFYCONSTRAINTACTIVITIESPREFERREDSTARTINGTIMESFORM_H

#include "ui_modifyconstraintactivitiespreferredstartingtimesform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class ModifyConstraintActivitiesPreferredStartingTimesForm : public QDialog, Ui::ModifyConstraintActivitiesPreferredStartingTimesForm_template  {
	Q_OBJECT

public:
	ConstraintActivitiesPreferredStartingTimes* _ctr;

	ModifyConstraintActivitiesPreferredStartingTimesForm(QWidget* parent, ConstraintActivitiesPreferredStartingTimes* ctr);
	~ModifyConstraintActivitiesPreferredStartingTimesForm();

	void updateTeachersComboBox();
	void updateStudentsComboBox(QWidget* parent);
	void updateSubjectsComboBox();
	void updateActivityTagsComboBox();

	void colorItem(QTableWidgetItem* item);

public slots:
	void ok();
	void cancel();
	
	void itemClicked(QTableWidgetItem* item);
	void horizontalHeaderClicked(int col);
	void verticalHeaderClicked(int row);
	
	void cellEntered(int row, int col);
	
	void setAllSlotsAllowed();
	void setAllSlotsNotAllowed();

	void durationCheckBox_toggled();
};

#endif
