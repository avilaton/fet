/***************************************************************************
                          modifyconstraintteachersminhoursdailyform.h  -  description
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

#ifndef MODIFYCONSTRAINTTEACHERSMINHOURSDAILYFORM_H
#define MODIFYCONSTRAINTTEACHERSMINHOURSDAILYFORM_H

#include "ui_modifyconstraintteachersminhoursdailyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class ModifyConstraintTeachersMinHoursDailyForm : public QDialog, Ui::ModifyConstraintTeachersMinHoursDailyForm_template  {
	Q_OBJECT
public:
	ConstraintTeachersMinHoursDaily* _ctr;

	ModifyConstraintTeachersMinHoursDailyForm(QWidget* parent, ConstraintTeachersMinHoursDaily* ctr);
	~ModifyConstraintTeachersMinHoursDailyForm();

	void updateMinHoursSpinBox();

public slots:
	void ok();
	void cancel();

	void allowEmptyDaysCheckBoxToggled();
};

#endif
