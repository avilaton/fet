/***************************************************************************
                          modifyconstraintteachermaxhourscontinuouslyform.h  -  description
                             -------------------
    begin                : July 19, 2007
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

#ifndef MODIFYCONSTRAINTTEACHERMAXHOURSCONTINUOUSLYFORM_H
#define MODIFYCONSTRAINTTEACHERMAXHOURSCONTINUOUSLYFORM_H

#include "ui_modifyconstraintteachermaxhourscontinuouslyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class ModifyConstraintTeacherMaxHoursContinuouslyForm : public QDialog, Ui::ModifyConstraintTeacherMaxHoursContinuouslyForm_template  {
	Q_OBJECT
public:
	ConstraintTeacherMaxHoursContinuously* _ctr;

	ModifyConstraintTeacherMaxHoursContinuouslyForm(QWidget* parent, ConstraintTeacherMaxHoursContinuously* ctr);
	~ModifyConstraintTeacherMaxHoursContinuouslyForm();

	void updateMaxHoursSpinBox();

public slots:
	void ok();
	void cancel();
};

#endif
