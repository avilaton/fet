/***************************************************************************
                          modifyconstraintteachersactivitytagmaxhourscontinuouslyform.h  -  description
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

#ifndef MODIFYCONSTRAINTTEACHERSACTIVITYTAGMAXHOURSCONTINUOUSLYFORM_H
#define MODIFYCONSTRAINTTEACHERSACTIVITYTAGMAXHOURSCONTINUOUSLYFORM_H

#include "ui_modifyconstraintteachersactivitytagmaxhourscontinuouslyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class ModifyConstraintTeachersActivityTagMaxHoursContinuouslyForm : public QDialog, Ui::ModifyConstraintTeachersActivityTagMaxHoursContinuouslyForm_template  {
	Q_OBJECT
public:
	ConstraintTeachersActivityTagMaxHoursContinuously* _ctr;

	ModifyConstraintTeachersActivityTagMaxHoursContinuouslyForm(QWidget* parent, ConstraintTeachersActivityTagMaxHoursContinuously* ctr);
	~ModifyConstraintTeachersActivityTagMaxHoursContinuouslyForm();

	void updateMaxHoursSpinBox();

public slots:
	void ok();
	void cancel();
};

#endif
