/***************************************************************************
                          modifyconstraintstudentsmaxgapsperweekform.h  -  description
                             -------------------
    begin                : Feb 11, 2005
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

#ifndef MODIFYCONSTRAINTSTUDENTSMAXGAPSPERWEEKFORM_H
#define MODIFYCONSTRAINTSTUDENTSMAXGAPSPERWEEKFORM_H

#include "ui_modifyconstraintstudentsmaxgapsperweekform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class ModifyConstraintStudentsMaxGapsPerWeekForm : public QDialog, Ui::ModifyConstraintStudentsMaxGapsPerWeekForm_template  {
	Q_OBJECT
public:
	ConstraintStudentsMaxGapsPerWeek* _ctr;

	ModifyConstraintStudentsMaxGapsPerWeekForm(QWidget* parent, ConstraintStudentsMaxGapsPerWeek* ctr);
	~ModifyConstraintStudentsMaxGapsPerWeekForm();

public slots:
	void ok();
	void cancel();
};

#endif
