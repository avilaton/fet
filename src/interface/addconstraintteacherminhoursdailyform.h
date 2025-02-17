/***************************************************************************
                          addconstraintteacherminhoursdailyform.h  -  description
                             -------------------
    begin                : Sept. 21, 2007
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

#ifndef ADDCONSTRAINTTEACHERMINHOURSDAILYFORM_H
#define ADDCONSTRAINTTEACHERMINHOURSDAILYFORM_H

#include "ui_addconstraintteacherminhoursdailyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintTeacherMinHoursDailyForm : public QDialog, Ui::AddConstraintTeacherMinHoursDailyForm_template  {
	Q_OBJECT
public:
	AddConstraintTeacherMinHoursDailyForm(QWidget* parent);
	~AddConstraintTeacherMinHoursDailyForm();

	void updateMinHoursSpinBox();

public slots:
	void addCurrentConstraint();
	void addCurrentConstraints();

	void allowEmptyDaysCheckBox_toggled();
};

#endif
