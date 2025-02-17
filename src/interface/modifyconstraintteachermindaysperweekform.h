/***************************************************************************
                          modifyconstraintteachermindaysperweekform.h  -  description
                             -------------------
    begin                : Feb 10, 2005
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

#ifndef MODIFYCONSTRAINTTEACHERMINDAYSPERWEEKFORM_H
#define MODIFYCONSTRAINTTEACHERMINDAYSPERWEEKFORM_H

#include "ui_modifyconstraintteachermindaysperweekform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class ModifyConstraintTeacherMinDaysPerWeekForm : public QDialog, Ui::ModifyConstraintTeacherMinDaysPerWeekForm_template  {
	Q_OBJECT
	
public:
	ConstraintTeacherMinDaysPerWeek* _ctr;

	ModifyConstraintTeacherMinDaysPerWeekForm(QWidget* parent, ConstraintTeacherMinDaysPerWeek* ctr);
	~ModifyConstraintTeacherMinDaysPerWeekForm();

	void updateMinDaysSpinBox();
	void updateTeachersComboBox();

public slots:
	void ok();
	void cancel();
};

#endif
