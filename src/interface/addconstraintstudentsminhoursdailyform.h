/***************************************************************************
                          addconstraintstudentsminhoursdailyform.h  -  description
                             -------------------
    begin                : 19 July 2007
    copyright            : (C) 2007 by Liviu Lalescu
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

#ifndef ADDCONSTRAINTSTUDENTSMINHOURSDAILYFORM_H
#define ADDCONSTRAINTSTUDENTSMINHOURSDAILYFORM_H

#include "ui_addconstraintstudentsminhoursdailyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintStudentsMinHoursDailyForm : public QDialog, Ui::AddConstraintStudentsMinHoursDailyForm_template  {
	Q_OBJECT
public:
	AddConstraintStudentsMinHoursDailyForm(QWidget* parent);
	~AddConstraintStudentsMinHoursDailyForm();

public slots:
	void addCurrentConstraint();
	
	void allowEmptyDaysCheckBox_toggled();
};

#endif
