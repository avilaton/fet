/***************************************************************************
                          addconstraintteacheractivitytagmaxhoursdailyform.h  -  description
                             -------------------
    begin                : 2009
    copyright            : (C) 2009 by Liviu Lalescu
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

#ifndef ADDCONSTRAINTTEACHERACTIVITYTAGMAXHOURSDAILYFORM_H
#define ADDCONSTRAINTTEACHERACTIVITYTAGMAXHOURSDAILYFORM_H

#include "ui_addconstraintteacheractivitytagmaxhoursdailyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintTeacherActivityTagMaxHoursDailyForm : public QDialog, Ui::AddConstraintTeacherActivityTagMaxHoursDailyForm_template  {
	Q_OBJECT
public:
	AddConstraintTeacherActivityTagMaxHoursDailyForm(QWidget* parent);
	~AddConstraintTeacherActivityTagMaxHoursDailyForm();

	void updateMaxHoursSpinBox();

public slots:
	void addCurrentConstraint();
	void addCurrentConstraints();
};

#endif
