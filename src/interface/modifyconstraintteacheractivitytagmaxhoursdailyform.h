/***************************************************************************
                          modifyconstraintteacheractivitytagmaxhoursdailyform.h  -  description
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

#ifndef MODIFYCONSTRAINTTEACHERACTIVITYTAGMAXHOURSDAILYFORM_H
#define MODIFYCONSTRAINTTEACHERACTIVITYTAGMAXHOURSDAILYFORM_H

#include "ui_modifyconstraintteacheractivitytagmaxhoursdailyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class ModifyConstraintTeacherActivityTagMaxHoursDailyForm : public QDialog, Ui::ModifyConstraintTeacherActivityTagMaxHoursDailyForm_template  {
	Q_OBJECT
public:
	ConstraintTeacherActivityTagMaxHoursDaily* _ctr;

	ModifyConstraintTeacherActivityTagMaxHoursDailyForm(QWidget* parent, ConstraintTeacherActivityTagMaxHoursDaily* ctr);
	~ModifyConstraintTeacherActivityTagMaxHoursDailyForm();

	void updateMaxHoursSpinBox();

public slots:
	void ok();
	void cancel();
};

#endif
